#pragma once

#include <string>
#include <memory>

struct IFooFactory {
  IFooFactory() = default;
  IFooFactory(const IFooFactory&) = delete;
  IFooFactory& operator=(const IFooFactory&) = delete;
  virtual ~IFooFactory() = default;

  virtual std::string show() = 0;
};

std::unique_ptr<IFooFactory> getFooFactory();
