#pragma once

#include <cstring>

namespace std
{
  constexpr const char* begin(const char* ptr) { return ptr; }

  constexpr size_t size(const char* ptr) { return ::strlen(ptr); }
}
