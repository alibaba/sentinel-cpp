#include <iostream>
#include <typeinfo>

#include "third_party/nlohmann/json.hpp"

int main() {
  try {
    auto j2 = nlohmann::json::parse("sa");
  } catch (std::exception& ex) {
    std::cerr << "Exception: " << ex.what() << std::endl;
  }

  // create a JSON array
  auto j3 = nlohmann::json::parse("{ \"happy\": true, \"pi\": 3.141 }");
  auto c = j3["pi"];
  if (c.is_number()) {
    int d = static_cast<int>(c);
    std::cout << d << std::endl;
  }

  std::cout << typeid(c).name() << std::endl;
  return 0;
}