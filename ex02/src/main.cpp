#include <algorithm>
#include <iostream>
#include <ranges>
#include <string_view>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error\n";
    return 1;
  }
  auto argv_range = std::ranges::subrange<char**>{argv + 1, argv + argc};

  auto str_is_num = [](const std::string_view& str) -> bool {};

  auto args_are_nums = std::ranges::all_of(argv_range, [](const std::string_view&) { return true; });
}
