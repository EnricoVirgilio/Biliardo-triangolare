#include <iostream>

#include "pool2.hpp"
int main() {
  pf::Parameters p = {120.f, 100.f, 500.f};
  pf::Pool pool{p};
  pool.draw_window();
  return 0;
}