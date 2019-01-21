#include "engine.h"
#include "chakra.h"
#include "core.h"
#include "time.h"
#include "http.h"
#include "console.h"

#include <seastar/core/reactor.hh>
#include <seastar/http/httpd.hh>
#include <seastar/http/handlers.hh>
#include <seastar/http/function_handlers.hh>
#include <seastar/http/file_handler.hh>

#include <iostream>

namespace ocn
{
  void promise_continuation_callback(chakra::ref task, void* state)
  {
    JsAddRef(task, nullptr);

    seastar::schedule(
      seastar::make_task(
        [task] {
          chakra::value t{task};
          t(chakra::global_object);
          JsRelease(task, nullptr);
        }));
  }

  bool thread_service(
    JsBackgroundWorkItemCallback callback, void* state)
  {
    seastar::schedule(
      seastar::make_task(
        [=] {
          callback(state);
        }));

    return true;
  }

  seastar::future<seastar::temporary_buffer<char>>
  read_file(const seastar::sstring& name)
  {
    return seastar::open_file_dma(name, seastar::open_flags::ro)
      .then([] (auto f) {
              return f.size()
                .then([f] (auto size) mutable {
                        return f.template dma_read_exactly<char>(0, size);
                      })
                .finally([f] {});
            });
  }

  engine::engine(const seastar::sstring& filename)
  {
    runtime_ = chakra::new_runtime(promise_continuation_callback, thread_service);

    time::set_up();
    console::set_up();
    core::set_up();
    http::set_up();

    run(filename);
  }

  seastar::future<> engine::stop()
  {
    chakra::dispose_runtime(runtime_);
    return seastar::make_ready_future<>();
  }

  seastar::future<> engine::run(const seastar::sstring& filename)
  {
    return read_file(filename)
      .then([=] (auto buffer) {
              try{
                auto source_context = seastar::engine().cpu_id();
                chakra::run(buffer, filename, source_context);
              } catch (const chakra::exception& e) {
                std::cerr << e.what() << std::endl;
                chakra::clear_exception();
              }
            });
  }
}
