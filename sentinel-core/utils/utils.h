#pragma once

namespace Sentinel {
namespace Utils {

template <class T>
class Singleton {
 public:
  /**
   * Obtain an instance of the singleton for class T.
   * @return const T& a reference to the singleton for class T.
   */
  static T& get() {
    static T* instance = new T();
    return *instance;
  }
};

}  // namespace Utils
}  // namespace Sentinel