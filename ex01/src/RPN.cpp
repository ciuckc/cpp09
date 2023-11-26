#include "RPN.h"

#include <cctype>
#include <functional>
#include <iostream>

namespace ft {
bool isSingleDigitNum(std::string::iterator it, std::string::iterator end) {
  if (*it == '-' || *it == '+') {
    ++it;
  }
  if (it == end) {
    return false;
  }
  return std::isdigit(*it);
}

bool isOperation(const std::string::iterator& it) {
  return *it == '-' || *it == '+' || *it == '*' || *it == '/';
}

int extractNumber(std::string::iterator& it) {
  int is_negative = (*it == '-' ? -1 : 1);
  if (*it == '-' || *it == '+') {
    ++it;
  }
  return is_negative * (*it - '0');
}
}  // namespace ft

RPN::RPN(const char* expr) : nums_(), expr_{expr} {}

void RPN::evaluateStackExpr(const std::string::iterator& it) {
  if (nums_.size() < 2) {
    std::cout << "Expression missing second operand!\n";
    std::exit(1);
  }
  auto second = nums_.top();
  nums_.pop();
  auto first = nums_.top();
  nums_.pop();
  if (*it == '+') {
    nums_.push(std::plus<int>{}(first, second));
  } else if (*it == '-') {
    nums_.push(std::minus<int>{}(first, second));
  } else if (*it == '*') {
    nums_.push(std::multiplies<int>{}(first, second));
  } else if (*it == '/') {
    nums_.push(std::divides<int>{}(first, second));
  }
}

void RPN::printExprResult() {
  auto it = expr_.begin();
  auto end = expr_.end();
  for (; it != end; ++it) {
    if (std::isspace(*it)) {
      continue;
    } else if (ft::isSingleDigitNum(it, end)) {
      nums_.push(ft::extractNumber(it));
    } else if (ft::isOperation(it)) {
      this->evaluateStackExpr(it);
    } else {
      std::cout << "Unknown operand!" << std::endl;
      return;
    }
  }
  std::cout << "result: " << nums_.top() << std::endl;
}
