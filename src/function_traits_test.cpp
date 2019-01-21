#include "function_traits.h"
#include "chakra_concepts.h"

#include <string>

namespace ocn::chakra
{
  int foo(double x)
  {
    return 3;
  }

  static_assert(!Callable<int>);

  auto lambda = [] { return 42; };
  static_assert(Functor<decltype(lambda)>);

  static_assert(function_traits<decltype(lambda)>::arity == 0);
  static_assert(Callable<decltype(lambda)>);

  static_assert(Function<decltype(foo)>);
  static_assert(Callable<decltype(foo)>);
}
