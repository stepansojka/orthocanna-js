#pragma once

#include "chakra_fwd.h"
#include "chakra_concepts.h"
#include "chakra_internal.h"
#include "chakra_exception.h"
#include "chakra_value.h"

#include <seastar/core/sstring.hh>
#include <seastar/core/temporary_buffer.hh>

namespace ocn::chakra
{
  ref new_runtime(JsPromiseContinuationCallback promise_callback,
                  JsThreadServiceCallback thread_service);

  void dispose_runtime(ref& runtime);

  ref new_external_object(void* data, JsFinalizeCallback finalize_callback = nullptr);

  template <String Code, String Filename>
  ref run(const Code& source_code,
          const Filename& script_filename,
          unsigned source_context)
  {
    ref js_script;
    invoke_checked(JsCreateString, std::begin(source_code),
                   std::size(source_code), &js_script);

    ref js_source_url;
    invoke_checked(JsCreateString, std::begin(script_filename),
                   std::size(script_filename), &js_source_url);

    ref result;
    invoke_checked(JsRun, js_script, source_context, js_source_url,
                   JsParseScriptAttributeNone, &result);
    return result;
  }

  template <String Code>
  ref eval(const Code& source_code)
  {
    static unsigned context = 0;
    return run(source_code, "", context++);
  }

  void set_prototype(ref object, ref prototype);

  seastar::sstring clear_exception() noexcept;
}
