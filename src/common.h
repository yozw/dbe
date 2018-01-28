#include <iostream>

template<class T> void Error(T message) {
  std::cerr << "ERROR: " << message << std::endl;
  exit(1);
}

template<class T> void PrintSet(T set) {
  for (auto i : set) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}
