#include "Boost.h"

#include <iostream>

#include "ProgramOptions/ProgramOptions.h"
#include "Spirit/Spirit.h"


void Boost::run()
{
  auto show = [] { std::cout << std::string(17, '=') + " ~~~ Boost ~~~ " + std::string(17, '=') << std::endl; };
  show();
  // return;

  ProgramOptions::run();
  Spirit::run();

  show();
}
