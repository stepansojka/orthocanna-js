#pragma once

#include <ChakraCore.h>

#include <exception>

namespace ocn::chakra
{
  typedef JsErrorCode error_code;

  const char* strerror(error_code c);

  struct exception : public std::exception
  {
    explicit exception(error_code c):
      code_(c)
      {}

    const char* what () const noexcept
    {
      return strerror(code_);
    }

    error_code code_;
  };
}
