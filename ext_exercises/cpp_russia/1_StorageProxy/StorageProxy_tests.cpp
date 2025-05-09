#include "StorageProxy_tests.h"
#include "StorageProxy.h"
#include <string>
#include <cassert>
#include <iostream>

namespace
{
  struct T {};
  struct B: T { int i{}; };
  class C { std::string s; };

  struct D {};
  struct E { int a = 9; int b = 8; int c = 7; };
}

void storageProxy_test_write();
void storageProxy_test_read();
void storageProxy_test()
{
  using namespace std;
  cout << "storageProxy_test_write::Begin " << endl;
  storageProxy_test_write();
  cout << "storageProxy_test_write::End " << endl;
  cout << endl;
  cout << endl;
  cout << "storageProxy_test_read::Begin " << endl;
  storageProxy_test_read();
  cout << "storageProxy_test_read::End " << endl;
}

void storageProxy_test_write()
{
  using namespace std;
  cout << "storageProxy_test: " << endl;
  cout << storageProxy::type_index<T>::value() << endl;
  cout << storageProxy::type_index<B>::value() << endl;
  cout << storageProxy::type_index<C>::value() << endl;
  cout << storageProxy::type_index<int>::value() << endl;
  cout << storageProxy::type_index<bool>::value() << endl;


  assert(storageProxy::type_index<T>::value() == 0);
  assert(storageProxy::type_index<B>::value() == 1);
  assert(storageProxy::type_index<C>::value() == 2);
  assert(storageProxy::type_index<int>() == 3);
  assert(storageProxy::type_index<bool>() == 4);

  // Alias
  using CAlias = C;
  assert(storageProxy::type_index<CAlias>::value() == 2);

  // Operator
  assert(storageProxy::type_index<T>() == 0);
  assert(storageProxy::type_index<B>() == 1);
  assert(storageProxy::type_index<C>() == 2);

  cout << endl;

  // Registry
  cout << "registry... " << endl;
  storageProxy::registry registry;

  registry.emplace(D{});
  registry.emplace(E{.a = 5, .b = 6, .c = 8});
  registry.emplace<E>(1, 2, 3);
}

void storageProxy_test_read()
{
  using namespace std;
  storageProxy::registry registry;
  registry.emplace(D{});
  registry.emplace(E{.a = 2, .b = 3, .c = 4});
  registry.emplace<E>(1, 2, 555);
  registry.emplace<double>(42.);
  registry.emplace<double>(555.);

  cout << "all_of<D>: " << registry.all_of<D>() << endl;
  cout << "all_of<E>: " << registry.all_of<E>() << endl;
  cout << "all_of<bool>: " << registry.all_of<bool>() << endl;

  cout << "all_of<D, E>: " << registry.all_of<D, E>() << endl;
  cout << "all_of<D, E, int>: " << registry.all_of<D, E, int>() << endl;
  cout << "all_of<D, E, double>: " << registry.all_of<D, E, double>() << endl;
  cout << endl;

  cout << "any_of<D>: " << registry.any_of<D>() << endl;
  cout << "any_of<E>: " << registry.any_of<E>() << endl;
  cout << "any_of<bool>: " << registry.any_of<bool>() << endl;

  cout << "any_of<D, E>: " << registry.any_of<D, E>() << endl;
  cout << "any_of<D, E, int>: " << registry.any_of<D, E, int>() << endl;
  cout << endl;


  const auto& arr = registry.all<double>();
  cout << "double: ";
  for (const auto& item : arr) {
    cout << item << "; ";
  }
  cout << endl;
  cout << endl;


  const auto& ld = registry.last<double>();
  cout << "last double: " << ld << endl;

  const auto& le = registry.last<E>();
  cout << "last E: " << le.a << "; " << le.b << "; " << le.c << endl;

  const auto& [dref, eref] = registry.last<double, E>();
  cout << endl;
  cout << "last double: " << dref << endl;
  cout << "last E: " << eref.a << "; " << eref.b << "; " << eref.c << endl;
}