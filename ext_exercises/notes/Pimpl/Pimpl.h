#pragma once

#include <memory>

class Pimpl {
public:
  Pimpl();
  ~Pimpl();
  Pimpl(Pimpl&& rhs);  
  Pimpl& operator=(Pimpl&& rhs);

  Pimpl(const Pimpl& rhs);
  Pimpl& operator=(const Pimpl& rhs);

  const char* get();

private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
