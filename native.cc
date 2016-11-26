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
typename star_unless_function_pointer<T>::type get_native(std::string module, std::string symbol) {
    // Fetch module, and open if not already open
    shlib so = fetch_module(module);
    if (!so) throw std::runtime_error("module " + module + " not found");

    void* sym = dlsym(so, symbol.data());
    if (!sym) throw std::runtime_error("symbol " + module + "::" + symbol + " not found");
    typedef typename std::remove_pointer<T>::type Tunstar;
    if (std::is_function<Tunstar>::value)
        return reinterpret_cast<T>(sym);
    return static_cast<T*>(sym);
}

template <typename RetType, typename... ArgType>
RetType call_native(std::string module, std::string function, ArgType... argv) {
    typedef RetType (*FuncType)(ArgType...);
    FuncType func = get_native<FuncType>(module, function);
    return func(argv...);
}
