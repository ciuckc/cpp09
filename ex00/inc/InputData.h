#include <fstream>

#include "PriceHistory.h"

class InputData {
 public:
  InputData(const char* file_name);
  InputData() = default;
  InputData(InputData&&) = default;
  InputData(const InputData&) = delete;
  InputData& operator=(InputData&&) = default;
  InputData& operator=(const InputData&) = delete;
  ~InputData() = default;

  void repl(const PriceHistory& history);

 private:
  void parseFilename(const char* file_name) const;
  bool parseDate(const std::string& date) const;
  bool parseValue(const std::string& value) const;
  void printResult(const PriceHistory& history, const std::string& date,
                   const std::string& value) const;

  std::ifstream file_{};
};
