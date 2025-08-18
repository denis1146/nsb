
#include "Pimpl/Pimpl.h"
#include <utility>
#include <iostream>

int main(int /* argc */, char* /* argv */[])
{
  // Pimpl =============================
  Pimpl p;
  Pimpl p1{ std::move(p) };
  p = std::move(p1);
  Pimpl p2{ p };
  p2 = p1;
  p2 = p;
  p2 = p;
  std::cout << p.get() << std::endl;

  return 0;
}