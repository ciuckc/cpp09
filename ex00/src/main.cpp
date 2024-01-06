#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "InputData.h"
#include "PriceHistory.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Error: could not open file." << std::endl;
    std::exit(1);
  }
  try {
    constexpr const char* data_filename = "data.csv";
    PriceHistory history_data(data_filename);
    InputData input_data(argv[1]);
    input_data.repl(history_data);
  } catch (const std::runtime_error& e) {
    std::cerr << "Exception:" << e.what() << '\n';
  }
}
