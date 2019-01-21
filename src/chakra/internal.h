#pragma once

#include "fwd.h"
#include "concepts.h"
#include "exception.h"

#include <type_traits>
#include <functional>
#include <utility>
#include <memory>
#include <cstring>
#include <iterator>
#include <initializer_list>

namespace ocn::chakra
{
  template <typename F, typename... Args>
  concept bool JsrtFunction = requires (F f, Args... args) {
    { f(args...) } -> JsErrorCode;
  };

  template <typename F, typename... Args>
    requires JsrtFunction<F, Args...>
  JsErrorCode invoke(F&& f, Args&&... args)
  {
    return std::forward<F>(f)(std::forward<Args>(args)...);
  }

  template <typename F, typename... Args>
    requires JsrtFunction<F, Args...>
  void invoke_checked(F&& f, Args&&... args)
  {
    JsErrorCode error_code = invoke(f, args...);
    if (error_code != JsNoError)
      throw exception(error_code);
  }

  template <typename T>
  auto delete_external(T)
  {
    static_assert(std::is_pointer<T>::value);
    return [] (auto p) { delete static_cast<T>(p); };
  }

  property_id_ref property_id(const char* name, size_t size);

  template <String T>
  property_id_ref property_id(const T& name)
  {
    return property_id(std::begin(name), std::size(name));
  }

  ref property(ref object, const char* name, size_t size);

  ref property(ref object, property_id_ref id);

  template <String T>
  ref property(ref object, const T& name)
  {
    return property(object, std::begin(name), std::size(name));
  }

  void set_property(ref object, const char* name, size_t size, ref property);

  void set_property(ref object, property_id_ref property_id, ref property);

  template <String T>
  void set_property(ref object, const T& name, ref property)
  {
    set_property(object, std::begin(name), std::size(name), property);
  }

  ref new_string(const char* s, size_t size);

  ref new_function(JsNativeFunction fn, void *callback_state);

  template <String T>
  ref new_string(const T& s)
  {
    return new_string(std::begin(s), std::size(s));
  }

  template <String T>
  T unwrap_string(ref object)
  {
    ref str;
    invoke_checked(JsConvertValueToString, object, &str);

    size_t len = 0;
    invoke_checked(JsCopyString, str, nullptr, 0, &len);

    T result;
    result.resize(len);
    invoke_checked(JsCopyString, str, result.data(), len, nullptr);
    return result;
  }

  template<typename T>
  ref call(ref fn, T args)
  {
    JsValueRef result;
    invoke_checked(JsCallFunction, fn, static_cast<void**>(std::begin(args)),
                   std::end(args) - std::begin(args), &result);
    return result;
  }

  ref new_object();

  template <Int T>
  ref new_int(T i)
  {
    ref result;
    invoke_checked(JsIntToNumber, i, &result);
    return result;
  }

  int unwrap_int(ref object);
}
