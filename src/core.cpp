#include "core.h"
#include "chakra/chakra.h"

#include <seastar/core/reactor.hh>
#include <seastar/core/sleep.hh>
#include <seastar/http/httpd.hh>
#include <seastar/http/handlers.hh>
#include <seastar/http/function_handlers.hh>
#include <seastar/http/file_handler.hh>

#include <utility>

namespace ocn::core
{
  using seastar::smp;

  static thread_local chakra::value on_smp_message_ = chakra::null;

  void core_receive(unsigned sender, const seastar::sstring& msg)
  {
    if (on_smp_message_)
      on_smp_message_(chakra::global_object, sender, msg);
  }

  void core_on_message(chakra::value that, chakra::value fn)
  {
    if (on_smp_message_)
      chakra::invoke(JsRelease, on_smp_message_, nullptr);

    on_smp_message_ = fn;

    chakra::invoke(JsAddRef, on_smp_message_, nullptr);
  }

  void core_send_to(chakra::value that, unsigned core_id, seastar::sstring message)
  {
    smp::submit_to(
      core_id, [sender = seastar::engine().cpu_id(), message = std::move(message)] {
                 core_receive(sender, message);
               });
  }

  void core_send_to_all(chakra::value that, seastar::sstring message)
  {
    smp::invoke_on_all(
      [sender = seastar::engine().cpu_id(), message = std::move(message)] {
        core_receive(sender, message);
      });
  }

  void core_send_to_others(chakra::value that, seastar::sstring message)
  {
    smp::invoke_on_all(
      [sender = seastar::engine().cpu_id(), message = std::move(message)] {
        if (seastar::engine().cpu_id() != sender)
          core_receive(sender, message);
      });
  }

  void set_up()
  {
    chakra::value core;
    core["number"] = [] {
                       return seastar::engine().cpu_id();
                     };

    core["onMessage"] = core_on_message;
    core["sendTo"] = core_send_to;
    core["sendToAll"] = core_send_to_all;
    core["sendToOthers"] = core_send_to_others;

    chakra::global_object["core"] = core;
  }
}
