#pragma once

#include "chakra/chakra.h"

#include <seastar/core/reactor.hh>

namespace ocn
{
  class engine
  {
  public:
    explicit engine(const seastar::sstring& filename);

    seastar::future<> run(const seastar::sstring& filename);

    seastar::future<> stop();

  private:
    JsRuntimeHandle runtime_;
  };
}
