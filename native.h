#include <string>

typedef void* shlib;

// Calls a native function and returns its result.
//
// The function is pulled from the file "libmodule.so" in the working directory, replacing module with the value of the module parameter.
// Throws a runtime_error if the module or function cannot be found.
template <typename RetType, typename... ArgType>
RetType call_native(std::string module, std::string function, ArgType... argv);
