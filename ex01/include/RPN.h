#pragma once

#include <stack>
#include <string>

class RPN {
 public:
  RPN(const char* expr);

  void printExprResult();

 private:
  std::stack<int> nums_;
  std::string expr_;

  void evaluateStackExpr(const std::string::iterator& it);
};
