#include <algorithm>
#include <cctype>
#include <charconv>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <optional>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace ft {
std::optional<int> is_positive_int_opt(std::string_view str) {
  int num{};
  auto trim_view = str | std::views::drop_while([](auto&& c) { return std::isspace(c); });
  if (trim_view.empty() || !std::isdigit(trim_view.front())) {
    return std::nullopt;
  }
  auto [result, err_code] = std::from_chars(trim_view.data(), trim_view.data() + trim_view.size(), num);
  if (err_code == std::errc::invalid_argument) {
    std::cerr << "Error: invalid_argument\n";
    return std::nullopt;
  } else if (err_code == std::errc::result_out_of_range) {
    std::cerr << "Error: out_of_range\n";
    return std::nullopt;
  }
  // result will point at the first char that is not matching the pattern or if
  // successfull it will point at the last char in the str
  if (num < 0 || result != trim_view.data() + trim_view.size()) {
    std::cerr << "Error: invalid_argument\n";
    return std::nullopt;
  }
  return num;
}

void insert_sort(std::list<std::pair<int, int>>& seq, const int size) {
  if (size == 1) {
    return;
  }
  insert_sort(seq, size - 1);
  const auto last_pair = *std::next(seq.begin(), size - 1);
  auto i = size - 2;
  for (; i >= 0 && std::next(seq.begin(), i)->second > last_pair.second; --i) {
    *std::next(seq.begin(), i + 1) = *std::next(seq.begin(), i);
  }
  *std::next(seq.begin(), i + 1) = last_pair;
}

void insert_sort(std::vector<std::pair<int, int>>& seq, const int size) {
  if (size == 1) {
    return;
  }
  insert_sort(seq, size - 1);
  const auto last_pair = seq[size - 1];
  auto i = size - 2;
  for (; i >= 0 && seq[i].second > last_pair.second; --i) {
    seq[i + 1] = seq[i];
  }
  seq[i + 1] = last_pair;
}

int jacobstahl(int i) {
  if (i == 0 || i == 1) {
    return i;
  }
  return jacobstahl(i - 1) + 2 * jacobstahl(i - 2);
}

void merge_sort(std::list<int>& main_chain, std::list<int>& pend) {
  main_chain.splice(main_chain.begin(), pend, pend.begin());
  if (pend.empty()) {
    return;
  }
  for (auto i = 2; i < std::ssize(pend); ++i) {
    auto current_jacob = jacobstahl(i);
    if (current_jacob > std::ssize(pend)) {
      break;
    }
    auto jacob_it = std::next(pend.begin(), current_jacob - 1);
    auto it = std::lower_bound(main_chain.begin(), main_chain.end(), *jacob_it);
    main_chain.insert(it, *jacob_it);
    *jacob_it = -1;
  }
  for (auto num : pend) {
    if (num != -1) {
      auto it = std::lower_bound(main_chain.begin(), main_chain.end(), num);
      main_chain.insert(it, num);
    }
  }
}

void merge_sort(std::vector<int>& main_chain, std::vector<int>& pend) {
  main_chain.insert(main_chain.begin(), pend.front());
  pend.erase(pend.begin());
  if (pend.empty()) {
    return;
  }
  for (auto i = 2; i < std::ssize(pend); ++i) {
    auto current_jacob = jacobstahl(i);
    if (current_jacob > std::ssize(pend)) {
      break;
    }
    auto it = std::lower_bound(main_chain.begin(), main_chain.end(), pend[current_jacob - 1]);
    main_chain.insert(it, pend[current_jacob - 1]);
    pend[current_jacob - 1] = -1;
  }
  for (auto num : pend) {
    if (num != -1) {
      auto it = std::lower_bound(main_chain.begin(), main_chain.end(), num);
      main_chain.insert(it, num);
    }
  }
}

void vector_ford_johnson(std::vector<int>& rng) {
  if (rng.size() == 1) {
    return;
  }
  std::optional<int> save_last{};
  if (rng.size() % 2 != 0) {
    save_last = rng.back();
    rng.pop_back();
  }
  auto chunk_view = rng | std::views::chunk(2) | std::views::transform([](auto&& pair) {
                      return pair[1] < pair[0] ? std::pair{pair[1], pair[0]} : std::pair{pair[0], pair[1]};
                    });
  std::vector<std::pair<int, int>> pair_seq(chunk_view.begin(), chunk_view.end());
  ft::insert_sort(pair_seq, pair_seq.size());

  std::vector<int> main_chain;
  std::vector<int> pend;
  for (auto [first, second] : pair_seq) {
    main_chain.push_back(second);
    pend.push_back(first);
  }
  ft::merge_sort(main_chain, pend);
  if (save_last.has_value()) {
    auto it = std::lower_bound(main_chain.begin(), main_chain.end(), save_last.value());
    main_chain.insert(it, save_last.value());
  }
  rng = std::move(main_chain);
}

void list_ford_johnson(std::list<int> rng) {
  if (rng.size() == 1) {
    return;
  }
  std::optional<int> save_last{};
  if (rng.size() % 2 != 0) {
    save_last = rng.back();
    rng.pop_back();
  }
  auto chunk_view = rng | std::views::chunk(2);
  std::list<std::pair<int, int>> pair_list{};
  for (const auto& chunk : chunk_view) {
    auto first = chunk.front();
    auto second = *std::next(chunk.begin());
    if (first < second) {
      pair_list.push_back({first, second});
    } else {
      pair_list.push_back({second, first});
    }
  }

  ft::insert_sort(pair_list, pair_list.size());
  std::list<int> main_chain;
  std::list<int> pend;
  for (auto [first, second] : pair_list) {
    main_chain.push_back(second);
    pend.push_back(first);
  }
  ft::merge_sort(main_chain, pend);
  if (save_last.has_value()) {
    auto it = std::lower_bound(main_chain.begin(), main_chain.end(), save_last.value());
    main_chain.insert(it, save_last.value());
  }
}
}  // namespace ft

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error\n";
    return 1;
  }

  const auto argv_range = std::ranges::subrange<char**>{argv + 1, argv + argc};
  // Verify if args are positive numbers and convert them to integral
  auto is_null_opt = [](const std::optional<int>& opt_int) { return opt_int == std::nullopt; };
  auto nums_opt_range = argv_range | std::views::transform(ft::is_positive_int_opt);
  if (std::ranges::any_of(nums_opt_range, is_null_opt)) {
    return 1;
  }

  // convert from range of optional ints to range of ints
  auto unwrap_opt_int = [](const std::optional<int>& opt_int) { return opt_int.value(); };
  auto nums_range = nums_opt_range | std::views::transform(unwrap_opt_int);

  std::vector<int> nums_v(nums_range.begin(), nums_range.end());
  std::list<int> nums_l(nums_range.begin(), nums_range.end());
  std::cout << "Before:";
  for (auto& num : nums_v) {
    std::cout << ' ' << num;
  }
  std::cout << '\n';

  auto start = std::chrono::steady_clock::now();
  ft::vector_ford_johnson(nums_v);
  auto end = std::chrono::steady_clock::now();
  auto duration_microsec = std::chrono::duration<double>(end - start).count();

  std::cout << "After:";
  for (auto& num : nums_v) {
    std::cout << ' ' << num;
  }
  std::cout << '\n';
  std::cout << "Time to process a range of " << nums_v.size() << " with [std::vector]: " << std::fixed
            << std::setprecision(5) << duration_microsec << "s\n";
  start = std::chrono::steady_clock::now();
  ft::list_ford_johnson(nums_l);
  end = std::chrono::steady_clock::now();
  duration_microsec = std::chrono::duration<double>(end - start).count();
  std::cout << "Time to process a range of " << nums_l.size() << " with [std::list]: " << std::fixed
            << std::setprecision(5) << duration_microsec << "s\n";
}
