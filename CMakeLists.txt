cmake_minimum_required(VERSION 3.20)

project(ocean VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(json)

add_library(ocean_lib
    src/Ocean.cpp
    src/Cell.cpp
    src/Object.cpp
    src/Stone.cpp
    src/Coral.cpp
    src/Prey.cpp
    src/Predator.cpp
    src/Config.cpp
    src/Simulation.cpp
)

target_include_directories(ocean_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ocean_lib PUBLIC nlohmann_json::nlohmann_json)

add_executable(ocean src/main.cpp)
target_link_libraries(ocean PRIVATE ocean_lib)

enable_testing()

add_executable(ocean_tests tests/tests.cpp)
target_link_libraries(ocean_tests PRIVATE ocean_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(ocean_tests)
