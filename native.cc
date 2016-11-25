#include <dlfcn.h>
#include <exception>
#include <map>
#include <string>

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

// Calls a native function and returns its result.
//
// The function is pulled from the file "libmodule.so" in the working directory, replacing module with the value of the module parameter.
// Throws a runtime_error if the module or function cannot be found.
template <typename RetType, typename... ArgType>
RetType call_native(std::string module, std::string function, ArgType... argv) {
    // Fetch module, and open if not already open
    shlib so = fetch_module(module);
    if (!so) throw std::runtime_error("module " + module + " not found");

    typedef RetType (*FuncType)(ArgType...);
    FuncType func = reinterpret_cast<FuncType>(dlsym(so, function.data()));
    if (!func) throw std::runtime_error("function " + module + "::" + function + " not found");
    return func(argv...);
}
