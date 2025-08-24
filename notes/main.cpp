
#include <iostream>

#include "Pimpl/Pimpl.h"
#include "Factory/Factory.h"
#include "Boost/ProgramOptions/ProgramOptions.h"

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

  // Boost =============================
  auto boostSt = std::string(20, '=') + " Boost " + std::string(20, '=');
  std::cout << boostSt << std::endl;
  ProgramOptions::run();
  std::cout << boostSt << std::endl;
  // Boost =============================

  return 0;
}