#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Error: No expression provided.\n";
  }
  (void)argv;
  return 0;
}
