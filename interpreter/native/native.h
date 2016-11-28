#ifndef _NATIVE_H_
#define _NATIVE_H_

#include <dlfcn.h>
#include <exception>
#include <map>
#include <string>
#include <type_traits>

typedef void* shlib;

namespace {
    std::map<std::string, shlib> open_modules;

    shlib fetch_module(std::string module) {
        if (open_modules.count(module))
            return open_modules[module];

        shlib so = dlopen(("./lib" + module + ".so").data(), RTLD_NOW);
        if (so) open_modules[module] = so;
        return so;
    }
}

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
T* get_native(std::string module, std::string symbol) {
    // Fetch module, and open if not already open
    shlib so = fetch_module(module);
    if (!so) throw std::runtime_error("module " + module + " not found");

    void* sym = dlsym(so, symbol.data());
    if (!sym) throw std::runtime_error("symbol " + module + "::" + symbol + " not found");
    if (std::is_function<T>::value)
        return reinterpret_cast<T*>(sym);
    return static_cast<T*>(sym);
}

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
RetType call_native(std::string module, std::string function, ArgType... argv) {
    // Fun fact: if you're calling a C function, the standard technically says that FuncType should be defined with C language linkage (aka extern "C").
    // But you can't do extern "C" inside templates or functions, even if it's just a typedef, so fixing this is impossible in the general case.
    // This isn't a problem on "normal" architectures, so we can safely ignore this.
    // If we end up calling only one or two function signatures, and want to be really standards compliant, we can extern "C" typedef what we need at namespace scope.
    // For now, though, screw it.
    typedef RetType FuncType(ArgType...);

    FuncType* func = get_native<FuncType>(module, function);
    return func(argv...);
}

#endif
