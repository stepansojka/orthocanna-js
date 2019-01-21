#pragma once

#include "chakra/chakra.h"

#include <seastar/core/future.hh>

// TOOD: move this to chakra files
namespace ocn::chakra
{
  template <typename Func>
    value promise(Func&& fn)
    {
      ref promise_, resolve, reject;
      invoke_checked(JsCreatePromise, &promise_, &resolve, &reject);
      invoke_checked(JsAddRef, promise_, nullptr);

      seastar::futurize_apply(fn, value(resolve), value(reject))
        .finally([=] {
            invoke_checked(JsRelease, promise_, nullptr);
          });

      return promise_;
    }
}

