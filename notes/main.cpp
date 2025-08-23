
#include <iostream>

#include "Pimpl/Pimpl.h"
#include "Factory/Factory.h"

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

  // Factory ===========================
  std::cout << getFooFactory()->show() << std::endl;

  return 0;
}