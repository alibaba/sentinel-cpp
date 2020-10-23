/*
 * Copyright (c) 2014 Tim Starling
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <atomic>
#include <mutex>
#include <new>
#include <thread>
#include <vector>
#include "tbb/concurrent_hash_map.h"

namespace Sentinel {
namespace Param {

using AtomicIntSharedPtr = std::shared_ptr<std::atomic<int>>;

/**
 * ThreadSafeLRUCache is a thread-safe hashtable with a limited size. When
 * it is full, insert() evicts the least recently used item from the cache.
 *
 * The find() operation fills a ConstAccessor object, which is a smart pointer
 * similar to TBB's const_accessor. After eviction, destruction of the value is
 * deferred until all ConstAccessor objects are destroyed.
 *
 * The implementation is generally conservative, relying on the documented
 * behaviour of tbb::concurrent_hash_map. LRU list transactions are protected
 * with a single mutex. Having our own doubly-linked list implementation helps
 * to ensure that list transactions are sufficiently brief, consisting of only
 * a few loads and stores. User code is not executed while the lock is held.
 *
 * The acquisition of the list mutex during find() is non-blocking (try_lock),
 * so under heavy lookup load, the container will not stall, instead some LRU
 * update operations will be omitted.
 *
 * Insert performance was observed to degrade rapidly when there is a heavy
 * concurrent insert/evict load, mostly due to locks in the underlying
 * TBB::CHM. So if that is a possibility for your workload,
 * ThreadSafeScalableCache is recommended instead.
 */
template <class TKey, class THash = tbb::tbb_hash_compare<TKey>>
class ThreadSafeLRUCache {
  /**
   * The LRU list node.
   *
   * We make a copy of the key in the list node, allowing us to find the
   * TBB::CHM element from the list node. TBB::CHM invalidates iterators
   * on most operations, even find(), ruling out more efficient
   * implementations.
   */
  struct ListNode {
    ListNode() : m_prev(nullptr), m_next(nullptr) {}

    ListNode(const TKey& key) : m_key(key), m_prev(nullptr), m_next(nullptr) {}

    TKey m_key;
    std::shared_ptr<ListNode> m_prev;
    std::shared_ptr<ListNode> m_next;

    bool isInList() const {
      if (m_prev)
        return true;
      else
        return false;
    }
  };

  static std::shared_ptr<ListNode> OutOfListMarker;

 public:
  /**
   * The value that we store in the hashtable. The list node is allocated from
   * an internal object_pool. The ListNode* is owned by the list.
   */
  struct HashMapValue {
    HashMapValue() : m_listNode(nullptr) {}

    HashMapValue(int value, std::shared_ptr<ListNode> node) : m_listNode(node) {
      m_value = std::make_shared<std::atomic<int>>(value);
    }

    AtomicIntSharedPtr m_value;
    std::shared_ptr<ListNode> m_listNode;
  };

  typedef tbb::concurrent_hash_map<TKey, HashMapValue, THash> HashMap;
  typedef std::shared_ptr<HashMap> HashMapSharedPtr;
  typedef typename HashMap::const_accessor HashMapConstAccessor;
  typedef typename HashMap::accessor HashMapAccessor;
  typedef typename HashMap::value_type HashMapValuePair;
  // typedef std::pair<const TKey, std::atomic<int>> SnapshotValue;

 public:
  /**
   * Create a container with a given maximum size
   */
  explicit ThreadSafeLRUCache(size_t maxSize);

  ThreadSafeLRUCache(const ThreadSafeLRUCache& other) = delete;
  ThreadSafeLRUCache& operator=(const ThreadSafeLRUCache&) = delete;

  ~ThreadSafeLRUCache() { clear(); }

  /**
   * Find a value by key, and return it by filling the ConstAccessor, which
   * can be default-constructed. Returns true if the element was found, false
   * otherwise. Updates the eviction list, making the element the
   * most-recently used.
   */
  bool find(HashMapAccessor& ac, const TKey& key);
  bool find(HashMapConstAccessor& ac, const TKey& key);

  /**
   * Find a value by key without disturbing the eviction list.
   */
  bool silentFind(HashMapAccessor& ac, const TKey& key);
  bool silentFind(HashMapConstAccessor& ac, const TKey& key);

  /**
   * Increase the value of the givin key by value.
   * Require that the value type should be addable.
   * If the givin key does not exist, just insert <key, value> pair and
   * return false.
   */
  bool increase(const TKey& key, int value);
  bool increase(HashMapAccessor& ac, const TKey& key, int value);

  /**
   * Insert a value into the container. Both the key and value will be copied.
   * The new element will put into the eviction list as the most-recently
   * used.
   *
   * If there was already an element in the container with the same key, it
   * will not be updated, and false will be returned. Otherwise, true will be
   * returned.
   */
  bool insert(const TKey& key, int value);
  bool insert(HashMapAccessor& ac, const TKey& key, int value);

  /**
   * Clear the container.
   */
  void clear();

  /**
   * Get a snapshot of the keys in the container by copying them into the
   * supplied vector. This will block inserts and prevent LRU updates while it
   * completes. The keys will be inserted in order from most-recently used to
   * least-recently used.
   */
  void snapshotKeys(std::vector<TKey>& keys);
  void snapshotPairs(std::vector<std::pair<TKey, int>>& pairs);

  /**
   * Get the approximate size of the container. May be slightly too low when
   * insertion is in progress.
   */
  size_t size() const { return m_size.load(); }

 private:
  /**
   * Unlink a node from the list. The caller must lock the list mutex while
   * this is called.
   */
  void delink(std::shared_ptr<ListNode> node);

  /**
   * Add a new node to the list in the most-recently used position. The caller
   * must lock the list mutex while this is called.
   */
  void pushFront(std::shared_ptr<ListNode> node);

  /**
   * Evict the least-recently used item from the container. This function does
   * its own locking.
   */
  void evict(const TKey& key);

  /**
   * The maximum number of elements in the container.
   */
  size_t m_maxSize;

  /**
   * This atomic variable is used to signal to all threads whether or not
   * eviction should be done on insert. It is approximately equal to the
   * number of elements in the container.
   */
  std::atomic<size_t> m_size;

  /**
   * The underlying TBB hash map.
   */
  HashMapSharedPtr m_map;

  /**
   * The linked list. The "head" is the most-recently used node, and the
   * "tail" is the least-recently used node. The list mutex must be held
   * during both read and write.
   */
  std::shared_ptr<ListNode> m_head = std::make_shared<ListNode>();
  std::shared_ptr<ListNode> m_tail = std::make_shared<ListNode>();
  std::mutex m_listMutex;
  std::mutex m_mapMutex;
};

// template <class TKey, class THash>
// typename ThreadSafeLRUCache<TKey, THash>::std::shared_ptr<ListNode> const
//     ThreadSafeLRUCache<TKey, THash>::OutOfListMarker = nullptr;

template <class TKey, class THash>
ThreadSafeLRUCache<TKey, THash>::ThreadSafeLRUCache(size_t maxSize)
    : m_maxSize(maxSize),
      m_size(0),
      m_map(std::make_unique<HashMap>(std::thread::hardware_concurrency() *
                                      4))  // it will automatically grow
{
  m_head->m_prev = nullptr;
  m_head->m_next = m_tail;
  m_tail->m_prev = m_head;
  m_tail->m_next = nullptr;
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::silentFind(HashMapConstAccessor& ac,
                                                 const TKey& key) {
  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  if (!local_map->find(ac, key)) {
    return false;
  }
  return true;
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::silentFind(HashMapAccessor& ac,
                                                 const TKey& key) {
  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  if (!local_map->find(ac, key)) {
    return false;
  }
  return true;
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::find(HashMapAccessor& ac,
                                           const TKey& key) {
  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  if (!local_map->find(ac, key)) {
    return false;
  }

  // Acquire the lock, but don't block if it is already held
  std::unique_lock<std::mutex> lock(m_listMutex);
  if (lock) {
    std::shared_ptr<ListNode> node = ac->second.m_listNode;
    // The list node may be out of the list if it is in the process of being
    // inserted or evicted. Doing this check allows us to lock the list for
    // shorter periods of time.
    if (node->isInList()) {
      delink(node);
      pushFront(node);
    }
    lock.unlock();
  }
  return true;
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::find(HashMapConstAccessor& ac,
                                           const TKey& key) {
  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  if (!local_map->find(ac, key)) {
    return false;
  }

  std::unique_lock<std::mutex> lock(m_listMutex);
  if (lock) {
    std::shared_ptr<ListNode> node = ac->second.m_listNode;
    if (node->isInList()) {
      delink(node);
      pushFront(node);
    }
    lock.unlock();
  }
  return true;
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::increase(const TKey& key, int value) {
  HashMapAccessor ac;
  return increase(ac, key, value);
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::increase(HashMapAccessor& ac,
                                               const TKey& key, int value) {
  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  if (!local_map->find(ac, key)) {
    // Key not found, insert the <key, value> pair.
    // When multiple threads reach here, only one thread successfully
    // inserts the key while others just increase the value on the
    // exist key.
    if (insert(ac, key, value)) {
      // Successfully inserted
      return false;
    }  // On fail return, `ac` is emplaced with ptr to the existing pair
  }
  ac->second.m_value->fetch_add(value);

  std::unique_lock<std::mutex> lock(m_listMutex);
  if (lock) {
    std::shared_ptr<ListNode> node = ac->second.m_listNode;
    // The list node may be out of the list if it is in the process of being
    // inserted or evicted. Doing this check allows us to lock the list for
    // shorter periods of time.
    if (node->isInList()) {
      delink(node);
      pushFront(node);
    }
    lock.unlock();
  }
  return true;
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::insert(const TKey& key, int value) {
  HashMapAccessor ac;
  return insert(ac, key, value);
}

template <class TKey, class THash>
bool ThreadSafeLRUCache<TKey, THash>::insert(HashMapAccessor& ac,
                                             const TKey& key, int value) {
  // Insert into the CHM
  std::shared_ptr<ListNode> node = std::make_shared<ListNode>(key);
  HashMapValuePair hashMapValue(key, HashMapValue(value, node));

  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  if (!local_map->insert(ac, hashMapValue)) {
    return false;
  }

  // Evict if necessary, now that we know the hashmap insertion was successful.
  size_t size = m_size.load();
  bool evictionDone = false;
  if (size >= m_maxSize) {
    // The container is at (or over) capacity, so eviction needs to be done.
    // Do not decrement m_size, since that would cause other threads to
    // inappropriately omit eviction during their own inserts.
    evict(key);
    evictionDone = true;
  }

  // Note that we have to update the LRU list before we increment m_size, so
  // that other threads don't attempt to evict list items before they even
  // exist.
  std::unique_lock<std::mutex> lock(m_listMutex);
  pushFront(node);
  lock.unlock();
  if (!evictionDone) {
    size = m_size++;
  }
  if (size > m_maxSize) {
    // It is possible for the size to temporarily exceed the maximum if there is
    // a heavy insert() load, once only as the cache fills. In this situation,
    // we have to be careful not to have every thread simultaneously attempt to
    // evict the extra entries, since we could end up underfilled. Instead we do
    // a compare-and-exchange to acquire an exclusive right to reduce the size
    // to a particular value.
    //
    // We could continue to evict in a loop, but if there are a lot of threads
    // here at the same time, that could lead to spinning. So we will just evict
    // one extra element per insert() until the overfill is rectified.
    if (m_size.compare_exchange_strong(size, size - 1)) {
      evict(key);
    }
  }
  return true;
}

template <class TKey, class THash>
void ThreadSafeLRUCache<TKey, THash>::clear() {
  std::unique_lock<std::mutex> mapLck(m_mapMutex, std::defer_lock);
  std::unique_lock<std::mutex> listLck(m_listMutex, std::defer_lock);
  lock(mapLck, listLck);
  m_map = std::make_unique<HashMap>(std::thread::hardware_concurrency() * 4);

  std::shared_ptr<ListNode> node = m_head->m_next;
  std::shared_ptr<ListNode> next;
  while (node != m_tail) {  // TODO: is '!=' ok?
    next = node->m_next;
    node = next;
  }
  m_head->m_next = m_tail;
  m_tail->m_prev = m_head;
  mapLck.unlock();
  listLck.unlock();
  m_size.store(0);
}

template <class TKey, class THash>
void ThreadSafeLRUCache<TKey, THash>::snapshotKeys(std::vector<TKey>& keys) {
  keys.reserve(keys.size() + m_size.load());
  std::unique_lock<std::mutex> lock(m_listMutex);
  for (std::shared_ptr<ListNode> node = m_head->m_next; node != m_tail;
       node = node->m_next) {
    keys.push_back(node->m_key);
  }
}

template <class TKey, class THash>
void ThreadSafeLRUCache<TKey, THash>::snapshotPairs(
    std::vector<std::pair<TKey, int>>& pairs) {
  pairs.reserve(pairs.size() + m_size.load());
  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  std::lock_guard<std::mutex> lock(m_listMutex);
  for (std::shared_ptr<ListNode> node = m_head->m_next; node != m_tail;
       node = node->m_next) {
    HashMapConstAccessor cac;
    if (!local_map->find(cac, node->m_key)) {
      // may have already been delinked
      continue;
    }
    pairs.push_back(std::make_pair<>(node->m_key, cac->second.m_value->load()));
  }
}

template <class TKey, class THash>
inline void ThreadSafeLRUCache<TKey, THash>::delink(
    std::shared_ptr<ListNode> node) {
  std::shared_ptr<ListNode> prev = node->m_prev;
  std::shared_ptr<ListNode> next = node->m_next;
  prev->m_next = next;
  next->m_prev = prev;
  node->m_prev = nullptr;
}

template <class TKey, class THash>
inline void ThreadSafeLRUCache<TKey, THash>::pushFront(
    std::shared_ptr<ListNode> node) {
  std::shared_ptr<ListNode> oldRealHead = m_head->m_next;
  node->m_prev = m_head;
  node->m_next = oldRealHead;
  oldRealHead->m_prev = node;
  m_head->m_next = node;
}

template <class TKey, class THash>
void ThreadSafeLRUCache<TKey, THash>::evict(const TKey& key) {
  std::unique_lock<std::mutex> lock(m_listMutex);
  std::shared_ptr<ListNode> moribund = m_tail->m_prev;
  if (!(moribund->m_prev) || THash().equal(moribund->m_key, key)) {
    // List is empty, can't evict
    return;
  }
  delink(moribund);
  lock.unlock();

  std::unique_lock<std::mutex> lck(m_mapMutex);
  HashMapSharedPtr local_map = m_map;
  lck.unlock();

  HashMapAccessor hashAccessor;
  if (!local_map->find(hashAccessor, moribund->m_key)) {
    // Presumably unreachable
    return;
  }
  local_map->erase(hashAccessor);
}

}  // namespace Param
}  // namespace Sentinel