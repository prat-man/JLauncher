#ifndef MAIN_H
#define MAIN_H

// WINDOWS
#ifdef WIN32
#define LIBTYPE HINSTANCE
#define OPENLIB(libname) LoadLibraryA((libname))
#define LIBFUNC(lib, fn) GetProcAddress((lib), (fn))
#define JVMLIB "/bin/server/jvm.dll"

// LINUX
#elif __linux__
#include <dlfcn.h>
#define LIBTYPE void*
#define OPENLIB(libname) dlopen((libname), RTLD_LAZY)
#define LIBFUNC(lib, fn) dlsym((lib), (fn))
#define JVMLIB "/lib/server/libjvm.so"

// MACOS
#elif __APPLE__
#include <dlfcn.h>
#define LIBTYPE void*
#define OPENLIB(libname) dlopen((libname), RTLD_LAZY)
#define LIBFUNC(lib, fn) dlsym((lib), (fn))
#define JVMLIB "/lib/server/libjvm.dylib"

#endif

#endif //MAIN_H
