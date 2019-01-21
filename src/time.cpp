#include "time.h"

#include "promise.h"
#include "chakra.h"

#include <seastar/core/sleep.hh>

#include <chrono>

namespace ocn::time
{
  chakra::value time_sleep(chakra::value that, int timeout)
  {
    return chakra::promise(
      [=] (auto resolve, auto reject) {
        return seastar::sleep(std::chrono::seconds(timeout))
          .then([=] () mutable {
                  resolve(chakra::global_object);
                });
      });
  }

  void set_up()
  {
    chakra::value time;
    time["sleep"] = time_sleep;

    chakra::global_object["time"] = time;
  }
}
