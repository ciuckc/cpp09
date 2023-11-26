#include <iostream>

#include "RPN.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Error: No expression provided.\n";
  }
  RPN resolver(argv[1]);
  resolver.printExprResult();
  return 0;
}
