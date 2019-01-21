#include "chakra.h"

namespace ocn::chakra
{
  thread_local value global_object = JS_INVALID_REFERENCE;

  ref new_runtime(JsPromiseContinuationCallback promise_callback, JsThreadServiceCallback thread_service)
  {
    ref runtime;
    //invoke_checked(JsCreateRuntime, JsRuntimeAttributeDisableBackgroundWork, nullptr, &runtime);
    //invoke_checked(JsCreateRuntime, JsRuntimeAttributeNone, nullptr, &runtime);
    invoke_checked(JsCreateRuntime, JsRuntimeAttributeEnableExperimentalFeatures, nullptr, &runtime);
    //invoke_checked(JsCreateRuntime, JsRuntimeAttributeNone, thread_service, &runtime);

    JsContextRef context;
    invoke_checked(JsCreateContext, runtime, &context);
    invoke_checked(JsSetCurrentContext, context);
    invoke_checked(JsSetPromiseContinuationCallback, promise_callback, nullptr);

    ref result;
    invoke_checked(JsGetGlobalObject, &result);
    global_object = result;

    return runtime;
  }

  void dispose_runtime(ref& runtime)
  {
    invoke_checked(JsSetCurrentContext, JS_INVALID_REFERENCE);
    invoke_checked(JsDisposeRuntime, runtime);
    runtime = JS_INVALID_REFERENCE;
  }

  ref new_object()
  {
    ref result;
    invoke_checked(JsCreateObject, &result);
    return result;
  }

  ref new_string(const char* s, size_t size)
  {
    ref result;
    invoke_checked(JsCreateString, s, size, &result);
    return result;
  }

  ref new_function(JsNativeFunction fn, void *callback_state)
  {
    ref result;
    invoke_checked(JsCreateFunction, fn, callback_state, &result);
    return result;
  }

  property_id_ref property_id(const char* name, size_t size)
  {
    property_id_ref id;
    invoke_checked(JsCreatePropertyId, name, size, &id);
    return id;
  }

  ref property(ref object, const char* name, size_t size)
  {
    property_id_ref id = property_id(name, size);

    ref result;
    invoke_checked(JsGetProperty, object, id, &result);
    return result;
  }

  ref property(ref object, property_id_ref id)
  {
    ref result;
    invoke_checked(JsGetProperty, object, id, &result);

    return ref(result);
  }

  void set_property(ref object, const char* name, size_t size, ref property)
  {
    auto id = property_id(name, size);
    set_property(object, id, property);
  }

  void set_property(ref object, property_id_ref property_id, ref property)
  {
    invoke_checked(JsSetProperty, object, property_id, property, true);
  }

  ref call(ref fn, ref* args, unsigned short arg_count)
  {
    JsValueRef result;
    invoke_checked(JsCallFunction, fn, args, arg_count, &result);
    return result;
  }

  ref call(ref fn, std::initializer_list<ref> args)
  {
    JsValueRef result;
    invoke_checked(JsCallFunction, fn, const_cast<void**>(args.begin()), args.size(), &result);
    return result;
  }

  seastar::sstring clear_exception() noexcept
  {
    ref exc_ref;
    JsGetAndClearExceptionWithMetadata(&exc_ref);

    value exc{exc_ref};

    std::cerr << "source: " << cast<seastar::sstring>(exc["source"]) << std::endl;
    std::cerr << "url: " << cast<seastar::sstring>(exc["url"]) << std::endl;

    std::cerr << cast<seastar::sstring>(exc["exception"]) << std::endl;

    return "";
  }

  int unwrap_int(ref object)
  {
    ref number;
    invoke_checked(JsConvertValueToNumber, object, &number);

    int i = 0;
    invoke_checked(JsNumberToInt, number, &i);
    return i;
  }

  ref new_external_object(void* data, JsFinalizeCallback finalize_callback)
  {
    ref obj;
    invoke_checked(JsCreateExternalObject, data, finalize_callback, &obj);
    return obj;
  }

  void set_prototype(ref object, ref prototype)
  {
    invoke_checked(JsSetPrototype, object, prototype);
  }
}
