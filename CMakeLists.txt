# [[Root]]
cmake_minimum_required(VERSION 3.5)

project(Eightser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")


# [[Options]]
option(EIGHTSER_BUILD_SHARED_LIBS "Build shared libraies by Default" ON)
option(EIGHTSER_FULLY_ENABLE "Build with fully by Default" OFF)
option(EIGHTSER_RTTI_ENABLE "Build with RTTI by Default" ON)
option(EIGHTSER_UNION_ENABLE "Build with union support by Default" ON)
option(EIGHTSER_PTRTRACK_ENABLE "Build with pointer track by Default" ON)
option(EIGHTSER_FILESTREAM_ENABLE "Build with std::fstream by Default" ON)
option(EIGHTSER_TYPE_PORTABLE_ENABLE "Build with type portable by Default" ON)
option(EIGHTSER_ANY_SUPPORT_ENABLE "Build with std::any support by Default" ON)
option(EIGHTSER_GARBAGE_CHECK_ENABLE "Build with garbage check by Default" ON)
option(EIGHTSER_BUILD_TEST_LIBS "Build testing libraies by Default" OFF)


# [[Defaults]]
if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Binaries")
    if(APPLE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Binaries/EightestLauncher.app/Contents/Frameworks")
    else()
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Binaries")
    endif()
endif()

if(LINUX)
    set(EIGHTSER_RPATH "$ORIGIN")
elseif(APPLE)
    set(EIGHTSER_RPATH "@loader_path/../Frameworks")
endif()


# [[Binaries]]
if(EIGHTSER_BUILD_SHARED_LIBS)
    set(PROJECT_LIBS_TYPE SHARED)
else()
    set(PROJECT_LIBS_TYPE STATIC)
endif()


file(GLOB_RECURSE PROJECT_SOURCES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
add_library(Eightser ${PROJECT_LIBS_TYPE} ${PROJECT_SOURCES_FILES})
target_include_directories(Eightser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(EIGHTSER_BUILD_SHARED_LIBS)
    set_target_properties(Eightser PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
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


# [[Tests]]
if(EIGHTSER_BUILD_TEST_LIBS)
    file(GLOB_RECURSE PROJECT_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp")
    add_library(EightserTests ${PROJECT_LIBS_TYPE} ${PROJECT_TEST_SOURCES})

    if(PROJECT_IS_TOP_LEVEL)
        # you should manually download Eightest if not
        set(EIGHTEST_RUN_MODULE "EightserTests")
        add_subdirectory("Eightest")
    endif()

    target_link_libraries(EightserTests PUBLIC Eightser Eightest)
    target_include_directories(EightserTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test")
    set_target_properties(EightserTests PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "${EIGHTSER_RPATH}")

    # we should define this macro to enable additional checks for testing
    target_compile_definitions(Eightser PUBLIC "EIGHTSER_DEBUG")
endif()
