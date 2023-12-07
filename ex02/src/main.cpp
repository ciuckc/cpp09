#include <algorithm>
#include <charconv>
#include <iostream>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

std::optional<int> is_positive_int_opt(const std::string_view& str) {
  int num{};
  //  i need to get rid of spaces and other stuff
  auto [result, err_code] = std::from_chars(str.data(), str.data() + str.length(), num);
  if (err_code == std::errc::invalid_argument) {
    std::cerr << "Error: invalid_argument\n";
    return std::nullopt;
  } else if (err_code == std::errc::result_out_of_range) {
    std::cerr << "Error: out_of_range\n";
    return std::nullopt;
  }
  // result will point at the first char that is not matching the pattern or if
  // successfull it will point at the last char in the str
  if (num < 0 || result != str.data() + str.length()) {
    std::cerr << "Error: invalid_argument\n";
    return std::nullopt;
  }
  return num;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error\n";
    return 1;
  }

  const auto argv_range = std::ranges::subrange<char**>{argv + 1, argv + argc};
  // Verify if args are positive numbers and convert them to integral
  auto nums_opt_range = argv_range | std::views::transform(is_positive_int_opt);
  auto is_null_opt = [](const std::optional<int>& opt_int) { return opt_int == std::nullopt; };
  if (std::ranges::any_of(nums_opt_range, is_null_opt)) {
    return 1;
  }

  // convert from range of optional ints to range of ints
  auto opt_to_int = [](const std::optional<int>& opt_int) { return *opt_int; };
  auto nums_range = nums_opt_range | std::views::transform(opt_to_int);

  std::vector<int> nums(nums_range.begin(), nums_range.end());
  for (auto num : nums) {
    std::cout << num << ' ';
  }
}
