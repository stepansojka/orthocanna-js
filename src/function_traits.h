#pragma once

#include <tuple>
#include <iostream>
#include <functional>
#include <type_traits>

namespace ocn::chakra
{
  template <typename ReturnType, typename... Args>
    struct function_traits_impl
    {
      static constexpr size_t arity = sizeof...(Args);

      using ptr_type =  ReturnType(*)(Args...);

      typedef ReturnType return_type;

      typedef std::tuple<Args...> args;

      template <size_t i>
      struct arg
      {
        typedef typename std::tuple_element<i, args>::type type;
      };
    };

  template <typename T>
  concept bool Functor = requires { void(&T::operator()); };

  template <typename T>
  concept bool Function = std::is_function<T>::value;

  template <typename>
  struct function_traits;

  template <Functor T>
  struct function_traits<T>
    : public function_traits<decltype(&T::operator())>
  {};

  template <Function T>
  struct function_traits<T>
    : public function_traits<std::function<T>>
  {};

  template <typename ClassType, typename ReturnType, typename... Args>
  struct function_traits<ReturnType(ClassType::*)(Args...) const>
    : public function_traits_impl<ReturnType, Args...>
  {};

  template <typename ClassType, typename ReturnType, typename... Args>
  struct function_traits<ReturnType(ClassType::*)(Args...)>
    : public function_traits_impl<ReturnType, Args...>
  {};

  template <typename ReturnType, typename... Args>
  struct function_traits<ReturnType(*)(Args...)>
    : public function_traits_impl<ReturnType, Args...>
  {};
}
