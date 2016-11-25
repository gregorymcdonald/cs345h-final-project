#include "native.h"

#include <dlfcn.h>
#include <exception>
#include <map>

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
T get_native(std::string module, std::string symbol) {
    // Fetch module, and open if not already open
    shlib so = fetch_module(module);
    if (!so) throw std::runtime_error("module " + module + " not found");

    T sym = reinterpret_cast<T>(dlsym(so, symbol.data()));
    if (!sym) throw std::runtime_error("symbol " + module + "::" + symbol + " not found");
    return sym;
}

template <typename RetType, typename... ArgType>
RetType call_native(std::string module, std::string function, ArgType... argv) {
    typedef RetType (*FuncType)(ArgType...);
    FuncType func = get_native<FuncType>(module, function);
    return func(argv...);
}
