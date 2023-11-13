#pragma once

#include <chrono>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <string_view>

class PriceHistory {
 public:
  using HistoryMap = std::map<std::chrono::year_month_day, std::string>;

 public:
  PriceHistory(const char* file_name);
  PriceHistory() = default;
  PriceHistory(PriceHistory&&) = default;
  PriceHistory(const PriceHistory&) = delete;
  PriceHistory& operator=(PriceHistory&&) = default;
  PriceHistory& operator=(const PriceHistory&) = delete;
  ~PriceHistory() = default;

  HistoryMap::iterator begin();
  HistoryMap::const_iterator begin() const;
  HistoryMap::iterator end();
  HistoryMap::const_iterator end() const;
  bool empty() const;
  bool addData(const std::chrono::year_month_day& key,
               const std::string& value);
  bool addData(std::chrono::year_month_day&& key, std::string&& value);
  std::optional<std::string> getData(
      const std::chrono::year_month_day& key) const;

 private:
  void parseFilename(const char* file_name) const;
  void parse();
  std::chrono::year_month_day parseDate(const std::string& date) const;
  void parseValue(const std::string& value) const;

  // Im brace initializing these
  HistoryMap data_map_{};
  std::ifstream file_{};
};
