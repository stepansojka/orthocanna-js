#pragma once

#include "fwd.h"
#include "concepts.h"
#include "internal.h"

namespace ocn::chakra
{
  struct setter;

  template <LucidFunctor T>
  ref new_function(T&& fn);

  struct value
  {
    ref ref_;

    value():
      ref_(new_object())
    {}

    value(ref r):
      ref_(r)
    {}

    value(const value&) = default;

    value(value&&) = default;

    value& operator=(const value&) = default;

    value& operator=(value&&) = default;

    template<Int T>
    value(T i):
      ref_(new_int(i))
    {}

    template<String T>
    value(const T& s):
      ref_(new_string(s))
    {}

    template<Function T>
    value(T& fn):
      ref_(new_function(fn))
    {}

    template<LucidFunctor T>
    value(T&& fn):
      ref_(new_function(std::forward<T>(fn)))
    {}

    operator bool() const
    {
      return ref_ != JS_INVALID_REFERENCE;
    }

    template <Pointer T>
    operator T() const
    {
      void* v;
      chakra::invoke_checked(JsGetExternalData, ref_, &v);
      return static_cast<T>(v);
    }

    template <Int T>
    operator T() const { return unwrap_int(ref_); }

    template <String T>
    operator T() const { return unwrap_string<T>(ref_); };

    operator ref() const { return ref_; }

    template <typename... Args>
    value operator()(Args... args)
    {
      std::array<ref, sizeof...(args)> arr {{ value{args}... }};
      return chakra::call(ref_, arr);
    }

    template <typename T>
    value operator[](const T& prop) const
    {
      return property(ref_, prop);
    }

    template <typename T>
    setter operator[](const T& prop);
  };

  struct setter : public value
  {
    ref object_;
    property_id_ref property_;

    setter(ref r, property_id_ref id):
      value(property(r, id)),
      object_(r),
      property_(id)
    {}

    setter& operator=(value r)
    {
      set_property(object_, property_, r);
      return *this;
    }
  };

  struct args
  {
    ref* args_;
    size_t size_;

    args(ref* args, size_t size):
      args_(args),
      size_(size)
    {}

    ref begin() const { return args_[0]; }

    ref end() const { return args_[size_ - 1]; }

    value operator[](size_t i) const
    {
      return args_[i];
    }

    size_t size() const
    {
      return size_;
    }
  };

  template <typename T>
  setter value::operator[](const T& prop)
  {
    auto id = property_id(prop);
    return {ref_, id};
  }

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


  // template <Callable T>
  // ref new_function(T&& fn)
  // {
  //   typedef typename function_traits<T>::ptr_type ptr_type;
  //   return new_function(static_cast<ptr_type>(fn));
  // }

  // template <Callable T>
  // value function(T&& fn)
  // {
  //   return new_function(std::forward<T>(fn));
  // }

  template <typename T>
  T cast(ref obj)
  {
    return value(obj);
  }

  constexpr ref null = nullptr;

  extern thread_local value global_object;
}
