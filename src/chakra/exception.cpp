#include "exception.h"

namespace ocn::chakra
{
  const char* strerror(error_code c)
  {
    switch (c)
      {
      case JsNoError:
        return "no error";

        // API Usage
      case JsErrorCategoryUsage:
        return "generic API usage error";
      case JsErrorInvalidArgument:
        return "invalid argument";
      case JsErrorNullArgument:
        return "null argument not allowed";
      case JsErrorNoCurrentContext:
        return "missing current context";
      case JsErrorInExceptionState:
        return "engine in exception state";
      case JsErrorNotImplemented:
        return "not implemented";
      case JsErrorWrongThread:
        return "API called on wrong thread";
      case JsErrorRuntimeInUse:
        return "runtime still in use";
      case JsErrorBadSerializedScript:
        return "bad serialized script";
      case JsErrorInDisabledState:
        return "runtime is disabled";
      case JsErrorCannotDisableExecution:
        return "no support for reliable script interruprion";
      case JsErrorHeapEnumInProgress:
        return "heap enumeration in progress";
      case JsErrorArgumentNotObject:
        return "argument not an object";
      case JsErrorInProfileCallback:
        return "profile callback underway";
      case JsErrorInThreadServiceCallback:
        return "thread service callback underway";
      case JsErrorCannotSerializeDebugScript:
        return "cannot serialize scripts in debug context";
      case JsErrorAlreadyDebuggingContext:
        return "context already in debug mode state";
      case JsErrorAlreadyProfilingContext:
        return "context already profiling";
      case JsErrorIdleNotEnabled:
        return "iddle processing not enabled";
      case JsCannotSetProjectionEnqueueCallback:
        return "projection enqueue callback not accepted";
      case JsErrorCannotStartProjection:
        return "failed to start projection";
      case JsErrorInObjectBeforeCollectCallback:
        return "operation not supported in an object before collect callback";
      case JsErrorObjectNotInspectable:
        return "object is not inspectable";
      case JsErrorPropertyNotSymbol:
        return "property is not a symbol";
      case JsErrorPropertyNotString:
        return "property is not a string";
      case JsErrorInvalidContext:
        return "invalid context";
      case JsInvalidModuleHostInfoKind:
        return "invalid module host info kind";
      case JsErrorModuleParsed:
        return "module already parsed";
      case JsNoWeakRefRequired:
        return "weak reference not required - object not managed by GC";
      case JsErrorPromisePending:
        return "promise still in pending state";
      case JsErrorModuleNotEvaluated:
        return "module not evaluated yet";

        // Engine
      case JsErrorCategoryEngine:
        return "generic engine category error";
      case JsErrorOutOfMemory:
        return "out of memory";
      case JsErrorBadFPUState:
        return "failed to set FPU state";

        // Script
      case JsErrorCategoryScript:
        return "generic script-related error";
      case JsErrorScriptException:
        return "JavaScript exception occured";
      case JsErrorScriptCompile:
        return "failed to compile script";
      case JsErrorScriptTerminated:
        return "script was terminated due to a request to suspend the runtime";
      case JsErrorScriptEvalDisabled:
        return "eval() is disabled";

        // Fatal
      case JsErrorCategoryFatal:
        return "generic fatal error";
      case JsErrorFatal:
        return "fatal error";
      case JsErrorWrongRuntime:
        return "API called with object from different runtime";

        // Diagnostic Operation Errors
      case JsErrorCategoryDiagError:
        return "generic diagnostic operation error";
      case JsErrorDiagAlreadyInDebugMode:
        return "already in debug mode";
      case JsErrorDiagNotInDebugMode:
        return "not in debug mode";
      case JsErrorDiagNotAtBreak:
        return "not at a break";
      case JsErrorDiagInvalidHandle:
        return "debugging invalid handle";
      case JsErrorDiagObjectNotFound:
        return "object not found";
      case JsErrorDiagUnableToPerformAction:
        return "unable to perform requested action";
      default:
        return "unknown error";
      }
  }
}
