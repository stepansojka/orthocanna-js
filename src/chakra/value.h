#pragma once

#include "fwd.h"
#include "concepts.h"
#include "internal.h"
#include "args.h"

namespace ocn::chakra
{
  struct setter;

  template <LucidFunctor T>
  ref new_function(T&& fn);

  template <Function T>
  ref new_function(T& fn);

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
      return call(ref_, arr);
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

  template <typename T>
  setter value::operator[](const T& prop)
  {
    auto id = property_id(prop);
    return {ref_, id};
  }

  template <typename T>
  T cast(ref obj)
  {
    return value(obj);
  }
  constexpr ref null = nullptr;

  extern thread_local value global_object;
}

#include "function.h"
