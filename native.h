#ifndef _NATIVE_H_
#define _NATIVE_H_

#include <string>

// Returns T*, unless T is a function pointer, in which case returns T.
template <typename T>
struct star_unless_function_pointer {
    typedef T* type;
};
template <typename R, typename... A>
struct star_unless_function_pointer<R (*)(A...)> {
    typedef R (*type)(A...);
};

// Retrieves a symbol from a native file.
// For data symbols, returns a pointer to the data (U = T*).
// For function symbols, returns a function pointer (U = T).
//
// The symbol is pulled from the file "libmodule.so" in the working directory, replacing module with the value of the module parameter.
// Throws a runtime_error if the module or symbol cannot be found.
template <typename T>
typename star_unless_function_pointer<T>::type get_native(std::string module, std::string symbol);

// Calls a native function and returns its result.
//
// Equivalent to
// (get_native<RetType (*)(ArgType...)>(module, function))(argv...).
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
