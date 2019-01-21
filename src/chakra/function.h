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
}
