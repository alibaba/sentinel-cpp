#include "sentinel-core/param/statistic/any_cmp.h"
#include <iostream>

int main() {
  absl::any a;
  std::cout << a.has_value();

  std::cout << a.type().name() << std::endl;
  const std::string INT_TYPE_STR(typeid(3).name());

  return 0;
}