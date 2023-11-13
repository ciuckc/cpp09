#include "PriceHistory.h"

#include <array>
#include <chrono>
#include <ios>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

PriceHistory::PriceHistory(const char* file_name)
    : data_map_{}, file_{file_name} {
  this->parseFilename(file_name);
  if (file_.is_open() == false) {
    throw std::runtime_error("Couldn't open input file!");
  }
  parse();
}

PriceHistory::HistoryMap::iterator PriceHistory::begin() {
  return data_map_.begin();
}

PriceHistory::HistoryMap::const_iterator PriceHistory::begin() const {
  return data_map_.begin();
}

PriceHistory::HistoryMap::iterator PriceHistory::end() {
  return data_map_.end();
}

PriceHistory::HistoryMap::const_iterator PriceHistory::end() const {
  return data_map_.end();
}

bool PriceHistory::addData(const std::chrono::year_month_day& key,
                           const std::string& value) {
  return data_map_.insert(std::make_pair(key, value)).second;
}

bool PriceHistory::addData(std::chrono::year_month_day&& key,
                           std::string&& value) {
  return data_map_.insert(std::make_pair(std::move(key), std::move(value)))
      .second;
}

bool PriceHistory::empty() const {
  return this->data_map_.empty();
}

std::optional<std::string> PriceHistory::getData(
    const std::chrono::year_month_day& key) const {
  auto it = data_map_.lower_bound(key);
  if (it == data_map_.end()) {
    return std::prev(it)->second;
  }
  if (key == it->first) {
    return it->second;
  }
  return std::prev(it)->second;
}

void PriceHistory::parseFilename(const char* file_name) const {
  std::string_view file_name_view{file_name};

  if (file_name_view.size() < 5 ||
      file_name_view.compare(file_name_view.size() - 4, 4, ".csv")) {
    throw std::runtime_error("Invalid file extension [.csv]!");
  }
}

void PriceHistory::parse() {
  auto&& file = this->file_;
  std::string line{};
  std::getline(file, line);
  if (file.fail() && !file.eof()) {
    throw std::ios_base::failure("Unable to read from file!");
  } else if (file.eof()) {
    throw std::runtime_error("File is empty");
  }
  if (line != "date,exchange_rate") {
    throw std::runtime_error("Wrong header file format!");
  }
  while (true) {
    std::getline(file, line);
    if (file.fail() && !file.eof()) {
      throw std::ios_base::failure("Unable to read from file!");
    } else if (file.eof()) {
      break;
    }
    auto separator = line.find(',');
    if (separator == 0 || separator == line.npos) {
      throw std::runtime_error("Wrong key, value pair!");
    }
    // could ve done this better...
    std::string date = line.substr(0, separator);
    std::chrono::year_month_day ymd = parseDate(date);
    std::string value = line.substr(separator + 1, line.size());
    parseValue(value);
    if (addData(std::move(ymd), std::move(value)) == false) {
      throw std::runtime_error("Error while inserting [key,value] pair!");
    }
  }
  if (data_map_.empty()) {
    throw std::runtime_error("Map is empty!");
  }
}

std::chrono::year_month_day PriceHistory::parseDate(
    const std::string& date) const {
  std::stringstream date_stream{std::string{date}};
  std::array<char, 2> delim = {};
  enum YmdIndex : int { YEAR = 0, MONTH, DAY, SIZE };
  std::array<int, YmdIndex::SIZE> ymd = {};

  date_stream >> ymd[YmdIndex::YEAR] >> delim[0] >> ymd[YmdIndex::MONTH] >>
      delim[1] >> ymd[YmdIndex::DAY];
  if (date_stream.fail() || delim[0] != '-' || delim[1] != '-') {
    throw std::runtime_error("Invalid date format!");
  }
  std::chrono::year_month_day date_obj{std::chrono::year(ymd[0]),
                                       std::chrono::month(ymd[1]),
                                       std::chrono::day(ymd[2])};
  if (date_obj.ok() == false) {
    throw std::runtime_error("Invalid date!");
  }
  return date_obj;
}

void PriceHistory::parseValue(const std::string& value) const {
  std::stringstream value_stream{std::string(value)};
  float price{};
  value_stream >> price;
  if (value_stream.fail()) {
    throw std::runtime_error("Invalid value!");
  }
}
