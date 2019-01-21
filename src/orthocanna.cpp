#include "engine.h"

#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sleep.hh>
#include <seastar/core/file.hh>
#include <seastar/core/thread.hh>
#include <seastar/core/sharded.hh>

#include <iostream>

namespace bpo = boost::program_options;

int main(int argc, char* argv[])
{
  seastar::app_template app;

  app.add_positional_options({
      { "file", bpo::value<std::string>(), "js file to run", 1 }
    });

  seastar::sharded<ocn::engine> e;

  app.run_deprecated(
    argc, argv, [&e, argv] {
                  seastar::engine().at_exit([&] { return e.stop(); });

                  return e.start(seastar::sstring(argv[1]));
                });

  return 0;
}
