#include "native.h"

#include <dlfcn.h>
#include <exception>
#include <map>
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
