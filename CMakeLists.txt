# [[Module][Root]]
cmake_minimum_required(VERSION 3.16)

project(Eightser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")


# [[Module][Options]]
option(EIGHTSER_BUILD_SHARED_LIBS "Build shared libraies by Default" ON)
option(EIGHTSER_FULLY_ENABLE "Build with fully by Default" OFF)
option(EIGHTSER_RTTI_ENABLE "Build with RTTI by Default" ON)
option(EIGHTSER_UNION_ENABLE "Build with union support by Default" ON)
option(EIGHTSER_PTRTRACK_ENABLE "Build with pointer track by Default" ON)
option(EIGHTSER_FILESTREAM_ENABLE "Build with std::fstream by Default" ON)
option(EIGHTSER_TYPE_PORTABLE_ENABLE "Build with type portable by Default" ON)
option(EIGHTSER_ANY_SUPPORT_ENABLE "Build with std::any support by Default" ON)
option(EIGHTSER_GARBAGE_CHECK_ENABLE "Build with garbage check by Default" ON)
option(EIGHTSER_DEBUG "Build with diagnostic extentions by Default" OFF)
option(EIGHTSER_BUILD_TEST_LIBS "Build testing libraies by Default" OFF)


# [[Module][Defaults]]
if(EIGHTSER_BUILD_SHARED_LIBS)
    set(PROJECT_LIBS_TYPE SHARED)
else()
    set(PROJECT_LIBS_TYPE STATIC)
endif()

if(WIN32)
    set(EIGHTSER_MODULE_EXPORT "__declspec(dllexport)")
    set(EIGHTSER_MODULE_IMPORT "__declspec(dllimport)")
elseif(LINUX OR APPLE)
    set(EIGHTSER_MODULE_EXPORT "__attribute__((visibility(\"default\")))")
    set(EIGHTSER_MODULE_IMPORT "")
endif()

if(LINUX OR APPLE)
    set(EIGHTSER_VISIBILITY_HIDDEN C_VISIBILITY_PRESET hidden CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN ON)
endif()


# [[Launcher][Defaults]]
if(PROJECT_IS_TOP_LEVEL AND EIGHTSER_BUILD_TEST_LIBS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Binaries")

    if(WIN32 OR LINUX)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Binaries")
    elseif(APPLE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Frameworks")
    endif()
endif()


# [[Module][Binaries]]
file(GLOB_RECURSE PROJECT_SOURCES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
add_library(Eightser ${PROJECT_LIBS_TYPE} ${PROJECT_SOURCES_FILES})
target_include_directories(Eightser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


# [[Module][Configurations]]
if(EIGHTSER_BUILD_SHARED_LIBS)
    target_compile_definitions(Eightser PRIVATE "EIGHTSER_API=${EIGHTSER_MODULE_EXPORT}" INTERFACE "EIGHTSER_API=${EIGHTSER_MODULE_IMPORT}")

    if(LINUX OR APPLE)
        set_target_properties(Eightser PROPERTIES ${EIGHTSER_VISIBILITY_HIDDEN})
    endif()
else()
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_API=")
endif()

if(EIGHTSER_FULLY_ENABLE)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_FULLY_ENABLE")
endif()

if(EIGHTSER_RTTI_ENABLE)
    target_compile_options(Eightser PUBLIC -frtti)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_RTTI_ENABLE")
else()
    target_compile_options(Eightser PUBLIC -fno-rtti)
endif()

if(EIGHTSER_UNION_ENABLE)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_UNION_ENABLE")
endif()

if(EIGHTSER_PTRTRACK_ENABLE)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_PTRTRACK_ENABLE")
endif()

if(EIGHTSER_FILESTREAM_ENABLE)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_FILESTREAM_ENABLE")
endif()

if(EIGHTSER_TYPE_PORTABLE_ENABLE)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_TYPE_PORTABLE_ENABLE")
endif()

if(EIGHTSER_ANY_SUPPORT_ENABLE)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_ANY_SUPPORT_ENABLE")
endif()

if(EIGHTSER_GARBAGE_CHECK_ENABLE)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_GARBAGE_CHECK_ENABLE")
endif()

if(EIGHTSER_DEBUG)
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_DEBUG")
endif()


# [[Tests][Defaults]]
if(EIGHTSER_BUILD_TEST_LIBS)
    if(LINUX)
        set(EIGHTSER_RPATH "$ORIGIN")
    elseif(APPLE)
        set(EIGHTSER_RPATH "@loader_path/../Frameworks")
    endif()
endif()


# [[Tests][Binaries]]
if(EIGHTSER_BUILD_TEST_LIBS)
    file(GLOB_RECURSE PROJECT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")
    add_library(EightserTests SHARED ${PROJECT_TEST_SOURCES})

    target_link_libraries(EightserTests PUBLIC Eightser Eightest)
    target_include_directories(EightserTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test")
endif()


# [[Tests][Configurations]]
if(EIGHTSER_BUILD_TEST_LIBS)
    target_compile_options(EightserTests PRIVATE -Wno-uninitialized)

    if(LINUX OR APPLE)
        set_target_properties(EightserTests PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "${EIGHTSER_RPATH}" ${EIGHTSER_VISIBILITY_HIDDEN})
    endif()
endif()


# [[Launcher][Binaries]]
if(PROJECT_IS_TOP_LEVEL AND EIGHTSER_BUILD_TEST_LIBS)
    # you should manually download Eightest if not
    set(EIGHTEST_RUN_MODULE "EightserTests")
    add_subdirectory("Eightest")
endif()
