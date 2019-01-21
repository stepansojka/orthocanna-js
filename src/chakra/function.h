#include "fwd.h"

#include <functional>

namespace ocn::chakra
{
  template <typename Fn, std::size_t... I>
  auto invoke_with_argpack(
    Fn fn, void** args, unsigned short len, std::index_sequence<I...>)
  {
    if (len < sizeof...(I))
      throw std::runtime_error("not enough values");

    return std::invoke(*fn, value{args[I]}...);
  }

  template <
    typename ReturnType, typename... Args,
    typename Indices = std::make_index_sequence<sizeof...(Args)>
    >
  JsValueRef variadic_thunk(
    JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
    unsigned short argument_count, void *callback_state)
  {
    auto fn = reinterpret_cast<ReturnType(*)(Args...)>(callback_state);

    if constexpr(!std::is_same<ReturnType, void>::value)
    {
      ReturnType result = invoke_with_argpack(fn, arguments, argument_count, Indices{});
      return value(result);
    }

    invoke_with_argpack(fn, arguments, argument_count, Indices{});
    return nullptr;
  }

  template <typename ReturnType>
  JsValueRef thunk(
    JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
    unsigned short argument_count, void *callback_state)
  {
    auto fn = reinterpret_cast<void(*)(args)>(callback_state);

    if constexpr(!std::is_same<ReturnType, void>::value)
    {
      ReturnType result = std::invoke(fn, args(arguments, argument_count));
      return value(result);
    }

    std::invoke(fn, args(arguments, argument_count));
    return nullptr;
  }

  template<typename ReturnType, typename... Args>
  ref new_function(ReturnType(*fn)(Args...))
  {
    return new_function(variadic_thunk<ReturnType, Args...>, reinterpret_cast<void*>(fn));
  }

  template<typename ReturnType>
  ref new_function(ReturnType(*fn)(args))
  {
    return new_function(thunk<ReturnType>, reinterpret_cast<void*>(fn));
  }

  template<LucidFunctor Func, typename ReturnType, typename ClassType, typename... Args>
  ref new_function(Func&& fn, ReturnType(ClassType::*)(Args...) const)
  {
    using ptr_type =  ReturnType(*)(Args...);
    return new_function(static_cast<ptr_type>(fn));
  }

  template <LucidFunctor T>
  ref new_function(T&& fn)
  {
    return new_function(std::forward<T>(fn), &T::operator());
  }
}
