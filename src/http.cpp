#include "http.h"

#include "promise.h"
#include "chakra/chakra.h"

#include <seastar/http/httpd.hh>
#include <seastar/http/handlers.hh>
#include <seastar/http/function_handlers.hh>
#include <seastar/http/file_handler.hh>


namespace ocn::http
{
  static thread_local chakra::value server_prototype_;

  chakra::value create_server(chakra::value that)
  {
    static thread_local uint16_t server_id = 0;
    auto name = seastar::format("http-{}-{}", seastar::engine().cpu_id(), server_id++);
    auto server = new seastar::http_server(std::move(name));

    chakra::ref obj = chakra::new_external_object(server, chakra::delete_external(server));
    chakra::set_prototype(obj, server_prototype_);
    return obj;
  }

  chakra::value listen(seastar::http_server* that, int port)
  {
    return chakra::promise(
      [=] (auto resolve, auto reject) {
        return that->listen(port)
          .then([=] () mutable {
                  resolve(chakra::global_object);
                });
      });
  }

  chakra::value create_req(seastar::httpd::request* req)
  {
    chakra::value r;
    r["url"] = req->_url;
    r["httpVersion"] = req->_version;
    r["httpVersionMajor"] = req->http_version_major;
    r["httpVersionMinor"] = req->http_version_minor;

    return r;
  }

  chakra::value create_resp()
  {
    chakra::value r;
    r["status"] = 200;
    r["body"] = "";

    return r;
  }

  void on(seastar::http_server* that, seastar::sstring method,
          seastar::sstring path, chakra::value fn)
  {
    auto handler = new seastar::httpd::function_handler(
      [fn] (auto req, auto resp) mutable {
        try {
          auto js_req = create_req(req.get());
          auto js_resp = create_resp();

          auto ret = fn(chakra::global_object, js_req, js_resp);

          int status = js_resp["status"];
          resp->set_status(seastar::httpd::reply::status_type(status));
          resp->_content = js_resp["body"];
        } catch (const chakra::exception& e) {
          std::cerr << e.what() << std::endl;
          chakra::clear_exception();

          resp->set_status(
            seastar::httpd::reply::status_type::internal_server_error);
        }

        return seastar::make_ready_future<decltype(resp)>(std::move(resp));
      }, "");

    that->_routes.add(
      seastar::httpd::str2type(method),
      seastar::url(path), handler);
  }

  void set_up()
  {
    server_prototype_ = chakra::value();
    server_prototype_["listen"] = listen;
    server_prototype_["on"] = on;

    chakra::value constructor{create_server};
    constructor["prototype"] = server_prototype_;

    chakra::value http;
    http["Server"] = constructor;

    chakra::global_object["http"] = http;
  }
}
