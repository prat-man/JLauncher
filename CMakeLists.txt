# Minimum CMAKE version
cmake_minimum_required(VERSION 3.28)

# C++ standard
set(CMAKE_CXX_STANDARD 17)

# Project name
project(JLauncher)

# Application name
set(APP_NAME RxDoctor)

# Executable
IF (WIN32)
    add_executable(${APP_NAME} WIN32
            src/main.cpp
            include/main.h
            manifest/manifest.rc
            resources/${APP_NAME}/app.h
            resources/${APP_NAME}/resource.rc)
ELSE()
    add_executable(${APP_NAME}
            src/main.cpp
            include/main.h
            resources/${APP_NAME}/app.h
            resources/${APP_NAME}/resource.rc)
ENDIF()

# Static linking
target_link_libraries(${APP_NAME} -static)

# Strip executable
set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)

# Include headers
include_directories(include)

# Include resources
include_directories(resources/${APP_NAME})

# Include JNI headers
include_directories($ENV{JAVA_HOME}/include)
include_directories($ENV{JAVA_HOME}/include/win32)
include_directories($ENV{JAVA_HOME}/include/linux)

# Include wxWidgets
set(wxWidgets_ROOT_DIR $ENV{WX_WIDGETS_ROOT})
find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(${APP_NAME} ${wxWidgets_LIBRARIES})
endif()
