# Minimum CMAKE version
cmake_minimum_required(VERSION 3.28)

# C++ standard
set(CMAKE_CXX_STANDARD 17)

# Project name
project(JLauncher)

# Application name
set(APP_NAME RxDoctor)

# Executable
add_executable(${APP_NAME} WIN32
        src/main.cpp
        manifest/manifest.rc
        resources/${APP_NAME}/main.h
        resources/${APP_NAME}/resource.rc)

# Static linking
set(CMAKE_EXE_LINKER_FLAGS -static)

include_directories(resources/${APP_NAME})

# Include JNI headers
include_directories($ENV{JAVA_HOME}/include)
include_directories($ENV{JAVA_HOME}/include/win32)
