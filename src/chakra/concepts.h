#pragma once

#include "monkey_patching.h"

#include <type_traits>
#include <iterator>

namespace ocn::chakra
{
  template <typename T1, typename T2>
  concept bool ConvertibleTo = std::is_convertible<T1, T2>::value;

  template <typename T>
  concept bool Int = std::is_integral<T>::value;

  template <typename T>
  concept bool Unsigned = std::is_integral<T>::value && !std::is_signed<T>::value;

  template <typename T>
  concept bool Function = std::is_function<T>::value;

  template <typename T>
  concept bool Pointer = std::is_pointer<T>::value;

  template <typename T>
  concept bool LucidFunctor = requires { void(&T::operator()); };

  template <typename T>
  concept bool String = requires (T s) {
    { std::begin(s) } -> ConvertibleTo<const char*>;
    { std::size(s) } -> Unsigned;
  };
}
