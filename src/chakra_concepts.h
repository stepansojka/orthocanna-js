#pragma once

#include "function_traits.h"
#include "monkey_patching.h"

#include <type_traits>
#include <iterator>
#include <cstring>

namespace ocn::chakra
{
  template <typename T>
  concept bool Int = std::is_integral<T>::value;

  template <typename T>
  concept bool Unsigned = std::is_integral<T>::value && !std::is_signed<T>::value;

  template <typename T>
  concept bool Pointer = std::is_pointer<T>::value;

  template <typename T1, typename T2>
  concept bool ConvertibleTo = std::is_convertible<T1, T2>::value;

  // concept bool Buffer = requires (T b) {
  //   { std::begin(b) } -> ConvertibleTo<const char*>;
  //   { std::size(b) } -> Unsigned;
  // };

  // template <typename T>
  // concept bool LucidFunctor = requires { void(&T::operator()); };


  template <typename T>
  concept bool LucidFunctor = requires { void(&T::operator()); };

  template <typename T>
  concept bool String = requires (T s) {
    { std::begin(s) } -> ConvertibleTo<const char*>;
    { std::size(s) } -> Unsigned;
  };

  template <typename T>
  concept bool Callable = requires {
    function_traits<T>::arity >= 0;
    typename function_traits<T>::args;
    typename function_traits<T>::return_type;
  };
}
