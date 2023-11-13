#include "InputData.h"

#include <bits/chrono.h>

#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "PriceHistory.h"

namespace util {
std::chrono::year_month_day strtoymd(const std::string& date) {
  std::stringstream date_stream{std::string{date}};
  char delim{};
  std::array<int, 3> ymd = {};

  date_stream >> ymd[0] >> delim >> ymd[1] >> delim >> ymd[2];
  std::chrono::year_month_day date_obj{std::chrono::year(ymd[0]),
                                       std::chrono::month(ymd[1]),
                                       std::chrono::day(ymd[2])};
  return date_obj;
}
}  // namespace util
InputData::InputData(const char* file_name) : file_(file_name) {
  parseFilename(file_name);
  if (file_.is_open() == false) {
    throw std::runtime_error("Coudn't open input file!");
  }
}

void InputData::parseFilename(const char* file_name) const {
  std::string_view file_name_view{file_name};

  if (file_name_view.size() < 5 ||
      file_name_view.compare(file_name_view.size() - 4, 4, ".txt")) {
    throw std::runtime_error("Invalid file extension [.txt]!");
  }
}

void InputData::repl(const PriceHistory& history) {
  std::string line{};
  auto&& file = this->file_;
  (void)history;

  std::getline(file, line, '\n');
  if (file.fail() && !file.eof()) {
    throw std::runtime_error("Can't read from input file!");
  } else if (file.eof()) {
    throw std::runtime_error("Input file is empty!");
  }
  if (line != "date | value") {
    throw std::runtime_error("Bad header in input file!l");
  }
  while (true) {
    std::getline(file, line, '\n');
    if (file.fail() && !file.eof()) {
      throw std::runtime_error("Can't read from input file!");
    } else if (file.eof()) {
      break;
    }
    auto separator = line.find(" | ");
    if (separator == 0 || separator == line.npos) {
      std::cerr << "Error: Bad input => " << line << '\n';
      continue;
    }
    std::string date = line.substr(0, separator);
    if (parseDate(date) == false) {
      std::cerr << "Error: Bad input => " << date << '\n';
      continue;
    }
    std::string value = line.substr(separator + 3, line.size());
    if (parseValue(value) == false) {
      continue;
    }
    printResult(history, date, value);
  }
}

bool InputData::parseDate(const std::string& date) const {
  std::stringstream date_stream{std::string{date}};
  std::array<char, 2> delim = {};
  enum YmdIndex : int { YEAR = 0, MONTH, DAY, SIZE };
  std::array<int, YmdIndex::SIZE> ymd = {};

  date_stream >> ymd[YmdIndex::YEAR] >> delim[0] >> ymd[YmdIndex::MONTH] >>
      delim[1] >> ymd[YmdIndex::DAY];
  if (date_stream.fail() || delim[0] != '-' || delim[1] != '-') {
    return false;
  }
  std::chrono::year_month_day date_obj{std::chrono::year(ymd[YmdIndex::YEAR]),
                                       std::chrono::month(ymd[YmdIndex::MONTH]),
                                       std::chrono::day(ymd[YmdIndex::DAY])};
  if (date_obj.ok() == false) {
    return false;
  }
  return true;
}

bool InputData::parseValue(const std::string& value) const {
  std::stringstream value_stream{std::string(value)};
  float price{};
  value_stream >> price;
  if (value_stream.fail()) {
    std::cerr << "Error: Bad input => " << value << '\n';
    return false;
  }
  if (price < 0) {
    std::cerr << "Error: not a positive number.\n";
    return false;
  }
  if (price > 1000) {
    std::cerr << "Error: number too big\n";
    return false;
  }
  return true;
}

void InputData::printResult(const PriceHistory& history,
                            const std::string& date,
                            const std::string& value) const {
  std::chrono::year_month_day ymd = util::strtoymd(date);
  float coins = std::strtof(value.c_str(), nullptr);
  if (ymd < history.begin()->first) {
    std::cerr << "Error: no data about querried date!\n";
    return;
  }
  auto opt = history.getData(ymd);
  if (opt.has_value()) {
    std::cout << date << " => " << value << " = "
              << coins * std::strtof(opt.value().c_str(), nullptr) << '\n';
    return;
  }
  std::cerr << "Error: Bad operation!\n";
  return;
}
