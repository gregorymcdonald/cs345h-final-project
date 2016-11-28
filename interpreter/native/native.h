#ifndef _NATIVE_H_
#define _NATIVE_H_

#include <string>

// Retrieves a symbol from a native file.
//
// T is the type of the symbol as declared in the original library.
// So get_native<int> for an int and get_native<int(int)> for a function.
// For data symbols (T is anything but a function pointer), returns a pointer to the data.
// For function symbols (T is a function pointer), returns a function pointer.
//
// The symbol is pulled from the file "libmodule.so" in the working directory, replacing module with the value of the module parameter.
// Throws a runtime_error if the module or symbol cannot be found.
template <typename T>
T* get_native(std::string module, std::string symbol);

// Calls a native function and returns its result.
//
// Equivalent to
// (get_native<RetType(ArgType...)>(module, function))(argv...).
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
