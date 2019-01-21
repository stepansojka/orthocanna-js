#include "console.h"

#include "chakra/chakra.h"

#include <seastar/core/reactor.hh>

namespace ocn::console
{
  using seastar::smp;

  void console_log(chakra::args args)
  {
    seastar::sstring s;

    for (size_t i = 1; i < args.size(); ++i)
    {
      if (i > 1)
        s += " ";

      s += args[i];
    }

    smp::submit_to(
      0, [s = std::move(s)] {
           std::cout << s << std::endl;
         });
  }

  void set_up()
  {
    chakra::value console;
    console["log"] = console_log;

    chakra::global_object["console"] = console;
  }
}
