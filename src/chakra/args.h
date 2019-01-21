#pragma once

#include "fwd.h"

namespace ocn::chakra
{
  struct args
  {
    ref* args_;
    size_t size_;

    args(ref* args, size_t size);

    ref begin() const { return args_[0]; }

    ref end() const { return args_[size_ - 1]; }

    value operator[](size_t i) const;

    size_t size() const { return size_; }
  };
}
