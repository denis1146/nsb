#include "Pimpl.h"

#include <string>

struct Pimpl::Impl
{
  std::string st = std::string(20, '=') + " Pimpl " +
                   std::string(20, '=');
};

Pimpl::Pimpl()
: pImpl{ std::make_unique<Impl>() }
{}

Pimpl::~Pimpl() = default;
Pimpl::Pimpl(Pimpl&& rhs) = default;
Pimpl& Pimpl::operator=(Pimpl&& rhs) = default;

Pimpl::Pimpl(const Pimpl& rhs)
{
  if (rhs.pImpl)
    pImpl = std::make_unique<Impl>(*rhs.pImpl);
}

Pimpl& Pimpl::operator=(const Pimpl& rhs)
{
  if (&rhs != this) {
    if (!rhs.pImpl)
      pImpl.reset();
    else if (pImpl)
      *pImpl = *rhs.pImpl;
    else
      pImpl = std::make_unique<Impl>(*rhs.pImpl);
  }
  return *this;
}

const char* Pimpl::get()
{
  return pImpl->st.c_str();
}
