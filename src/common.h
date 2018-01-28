#include <chrono>
#include <iostream>

typedef std::chrono::high_resolution_clock Clock;

template <class T> void Error(T message) {
  std::cerr << "ERROR: " << message << std::endl;
  exit(1);
}

template <class T> void PrintSet(T set) {
  for (auto i : set) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}

template <class S, class T>
unsigned long GetMillisecondsSince(std::chrono::time_point<S, T> begin_time) {
  auto dt = Clock::now() - begin_time;
  auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(dt);
  return interval.count();
}
