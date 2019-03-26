#include <iostream>
#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"

std::string Greet(absl::string_view person) {
  return absl::StrCat("Hello ", person);
}

int main(int argc, char* argv[]) {
  std::cout << Greet(argc < 2 ? "world" : argv[1]) << std::endl;
}
