#include <ctime>
#include <random>

std::mt19937 mt(std::time(nullptr));

int getRandom(int min, int max) {
  std::uniform_int_distribution<int> dist{min, max};
  return dist(mt);
}