#include <boost/optional.hpp>

#include <algorithm>
#include <climits>
#include <iostream>

template <class T> unsigned long long CalcNum() {
  T n;
  unsigned long long num = 0;
  while (std::cin >> n) {
    ++num;
  }
  return num;
}

template <class T> boost::optional<T> CalcMin() {
  T n;
  T min = INT_MAX;
  bool has_value = false;
  while (std::cin >> n) {
    has_value = true;
    min = std::min(min, n);
  }
  return has_value ? boost::optional<T>{min} : boost::none;
}

template <class T> boost::optional<T> CalcMax() {
  T n;
  T max = INT_MIN;
  bool has_value = false;
  while (std::cin >> n) {
    has_value = true;
    max = std::max(max, n);
  }
  return has_value ? boost::optional<T>{max} : boost::none;
}

template <class T> void Print(T value) { std::cout << value << std::endl; }

template <class T> void PrintOptional(boost::optional<T> optional) {
  if (optional) {
    Print(*optional);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Calculates functions of large data sets." << std::endl;
    std::cerr << "Syntax: calc <operation>" << std::endl;
    std::cerr << "Valid operation: min, max, num" << std::endl;
    exit(0);
  }

  std::string op = argv[1];
  if (op == "min") {
    PrintOptional(CalcMin<long long int>());
  } else if (op == "max") {
    PrintOptional(CalcMax<long long int>());
  } else if (op == "num") {
    Print(CalcNum<long long int>());
  } else {
    std::cerr << "Invalid operation: " << op << std::endl;
  }
}