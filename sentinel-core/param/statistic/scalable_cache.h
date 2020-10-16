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

#include <limits>
#include <memory>
#include "lru_cache.h"

namespace Sentinel {
namespace Param {

/**
 * ThreadSafeScalableCache is a thread-safe sharded hashtable with limited
 * size. When it is full, it evicts a rough approximation to the least recently
 * used item.
 *
 * The find() operation fills a ConstAccessor object, which is a smart pointer
 * similar to TBB's const_accessor. After eviction, destruction of the value is
 * deferred until all ConstAccessor objects are destroyed.
 *
 * Since the hash value of each key is requested multiple times, you should use
 * a key with a memoized hash function. ThreadSafeStringKey is provided for
 * this purpose.
 */
template <class TKey, class THash = tbb::tbb_hash_compare<TKey>>
class ThreadSafeScalableCache {
 public:
  using Shard = ThreadSafeLRUCache<TKey, THash>;
  typedef typename Shard::HashMapValue HashMapValue;
  typedef typename Shard::HashMapAccessor HashMapAccessor;
  typedef typename Shard::HashMapConstAccessor HashMapConstAccessor;
  typedef tbb::concurrent_hash_map<TKey, HashMapValue, THash> HashMap;

  /**
   * Constructor
   *   - maxSize: the maximum number of items in the container
   *   - numShards: the number of child containers. If this is zero, the
   *     "hardware concurrency" will be used (typically the logical processor
   *     count).
   */
  explicit ThreadSafeScalableCache(size_t maxSize, size_t numShards = 0);

  ThreadSafeScalableCache(const ThreadSafeScalableCache&) = delete;
  ThreadSafeScalableCache& operator=(const ThreadSafeScalableCache&) = delete;

  /**
   * Find a value by key, and return it by filling the ConstAccessor, which
   * can be default-constructed. Returns true if the element was found, false
   * otherwise. Updates the eviction list, making the element the
   * most-recently used.
   */
  bool find(HashMapAccessor& ac, const TKey& key);
  bool find(HashMapConstAccessor& ac, const TKey& key);

  bool silentFind(HashMapAccessor& ac, const TKey& key);
  bool silentFind(HashMapConstAccessor& ac, const TKey& key);

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
   * Clear the container. NOT THREAD SAFE -- do not use while other threads
   * are accessing the container.
   */
  void clear();

  /**
   * Get a snapshot of the keys in the container by copying them into the
   * supplied vector. This will block inserts and prevent LRU updates while it
   * completes. The keys will be inserted in a random order.
   */
  void snapshotKeys(std::vector<TKey>& keys);
  void snapshotPairs(std::vector<std::pair<TKey, int>>& pairs);

  /**
   * Get the approximate size of the container. May be slightly too low when
   * insertion is in progress.
   */
  size_t size() const;

 private:
  /**
   * Get the child container for a given key
   */
  Shard& getShard(const TKey& key);

  /**
   * The maximum number of elements in the container.
   */
  size_t m_maxSize;

  /**
   * The child containers
   */
  size_t m_numShards;
  typedef std::shared_ptr<Shard> ShardPtr;
  std::vector<ShardPtr> m_shards;
};

/**
 * A specialisation of ThreadSafeScalableCache providing a cache with efficient
 * string keys.
 */

template <class TKey, class THash>
ThreadSafeScalableCache<TKey, THash>::ThreadSafeScalableCache(size_t maxSize,
                                                              size_t numShards)
    : m_maxSize(maxSize), m_numShards(numShards) {
  if (m_numShards == 0) {
    m_numShards = std::thread::hardware_concurrency();
  }
  for (size_t i = 0; i < m_numShards; i++) {
    size_t s = maxSize / m_numShards;
    if (i == 0) {
      s += maxSize % m_numShards;
    }
    m_shards.emplace_back(std::make_shared<Shard>(s));
  }
}

template <class TKey, class THash>
typename ThreadSafeScalableCache<TKey, THash>::Shard&
ThreadSafeScalableCache<TKey, THash>::getShard(const TKey& key) {
  THash hashObj;
  constexpr int shift = std::numeric_limits<size_t>::digits - 16;
  size_t h = (hashObj.hash(key) >> shift) % m_numShards;
  return *m_shards.at(h);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::silentFind(HashMapConstAccessor& ac,
                                                      const TKey& key) {
  return getShard(key).silentFind(ac, key);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::silentFind(HashMapAccessor& ac,
                                                      const TKey& key) {
  return getShard(key).silentFind(ac, key);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::find(HashMapConstAccessor& ac,
                                                const TKey& key) {
  return getShard(key).find(ac, key);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::find(HashMapAccessor& ac,
                                                const TKey& key) {
  return getShard(key).find(ac, key);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::increase(const TKey& key,
                                                    int value) {
  return getShard(key).increase(key, value);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::increase(HashMapAccessor& ac,
                                                    const TKey& key,
                                                    int value) {
  return getShard(key).increase(ac, key, value);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::insert(const TKey& key, int value) {
  return getShard(key).insert(key, value);
}

template <class TKey, class THash>
bool ThreadSafeScalableCache<TKey, THash>::insert(HashMapAccessor& ac,
                                                  const TKey& key, int value) {
  return getShard(key).insert(ac, key, value);
}

template <class TKey, class THash>
void ThreadSafeScalableCache<TKey, THash>::clear() {
  for (size_t i = 0; i < m_numShards; i++) {
    m_shards[i]->clear();
  }
}

template <class TKey, class THash>
void ThreadSafeScalableCache<TKey, THash>::snapshotKeys(
    std::vector<TKey>& keys) {
  for (size_t i = 0; i < m_numShards; i++) {
    m_shards[i]->snapshotKeys(keys);
  }
}

template <class TKey, class THash>
void ThreadSafeScalableCache<TKey, THash>::snapshotPairs(
    std::vector<std::pair<TKey, int>>& pairs) {
  for (size_t i = 0; i < m_numShards; i++) {
    m_shards[i]->snapshotPairs(pairs);
  }
}

template <class TKey, class THash>
size_t ThreadSafeScalableCache<TKey, THash>::size() const {
  size_t size;
  for (size_t i = 0; i < m_numShards; i++) {
    size += m_shards[i]->size();
  }
  return size;
}

}  // namespace Param
}  // namespace Sentinel