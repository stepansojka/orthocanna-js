#pragma once

#include "exception.h"

#include <ChakraCore.h>

#include <utility>
#include <functional>

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
      return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

  template <typename F, typename... Args>
    requires JsrtFunction<F, Args...>
    void invoke_checked(F&& f, Args&&... args)
    {
      JsErrorCode error_code = invoke(std::forward<F>(f), std::forward<Args>(args)...);
      if (error_code != JsNoError)
        throw exception(error_code);
    }
}
