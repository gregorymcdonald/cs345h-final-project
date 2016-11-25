#ifndef _NATIVE_H_
#define _NATIVE_H_

#include <string>

// Calls a native function and returns its result.
//
// The function is pulled from the file "libmodule.so" in the working directory, replacing module with the value of the module parameter.
// Throws a runtime_error if the module or function cannot be found.
//
// WARNING: Your method call may fail spectacularly unless all of the ArgTypes and the RetType are standard layout types.
// http://en.cppreference.com/w/cpp/concept/StandardLayoutType
// In particular, references (like int&) and standard library types (like std::vector) are NOT standard layout types.
template <typename RetType, typename... ArgType>
RetType call_native(std::string module, std::string function, ArgType... argv);

#endif
