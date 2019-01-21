#include "args.h"

#include "value.h"

namespace ocn::chakra
{
  args::args(ref* args, size_t size):
    args_(args),
    size_(size)
  {}

  value args::operator[](size_t i) const
  {
    return args_[i];
  }
}
