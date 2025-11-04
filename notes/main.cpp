
#include <iostream>

#include "Boost/Boost.h"
#include "Factory/Factory.h"
#include "Pimpl/Pimpl.h"
#include "Ranges/RangesTest.h"
#include "SimpleAny/SimpleAny.h"
#include "ScopeExit/ScopeExit.h"
#include "Transformations/Transformations.h"
#include "TemplateTest/TemplateTest.h"

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
  Boost::run();

  // SimpleAny =========================
  SimpleAny::run();

  // Ranges ============================
  RangesTest::run();

  // ScopeExit =========================
  ScopeExit{}.run();

  // Transformations ==================
  Transformations::run();

  // TemplateTest ==================
  TemplateTest::run();

  return 0;
}