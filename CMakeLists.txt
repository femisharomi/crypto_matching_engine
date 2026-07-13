# ==============================================================================
# 1. CMAKE & PROJECT CONFIGURATION
# ==============================================================================

# Enforces the minimum CMake version required to parse this file safely.
cmake_minimum_required(VERSION 3.20)

# Defines the project name and sets the programming language to C++ (CXX).
project(crypto_matching_engine LANGUAGES CXX)

# Force the compiler to use modern C++20 standards without non-standard extensions.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================================================================
# 2. CORE LIBRARY CONFIGURATION (THE ENGINE)
# ==============================================================================

# Creates a library target named 'cme_core' to hold the main engine logic.
add_library(cme_core
    software/src/cme_core.cpp
    software/src/order/order.cpp
    software/src/order/order_validation.cpp
)

# Specifies where the header (.h/.hpp) files for 'cme_core' are located.
# PUBLIC means any target linking to cme_core will also see these headers.
target_include_directories(cme_core 
    PUBLIC 
        software/include
)

# ==============================================================================
# 3. MAIN APPLICATION CONFIGURATION
# ==============================================================================

# Creates the main runnable program executable named 'cme_app'.
add_executable(cme_app
    software/app/main.cpp
)

# Links 'cme_app' with 'cme_core' so the app can use the library's features.
target_link_libraries(cme_app
    PRIVATE
        cme_core
)

# ==============================================================================
# 4. EXTERNAL DEPENDENCY: GOOGLETEST
# ==============================================================================

# Tells CMake to enable its built-in testing features (enables the 'make test' command).
enable_testing()

# Includes the CMake module required to download external code at build time.
include(FetchContent)

# Declares the location and version of GoogleTest to download from GitHub.
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Downloads, configures, and builds GoogleTest isolated inside the project.
FetchContent_MakeAvailable(googletest)

# ==============================================================================
# 5. TEST RUNNER CONFIGURATION
# ==============================================================================

# Creates a separate executable target specifically for running your unit tests.
add_executable(cme_tests
    software/tests/core/test_types.cpp
    software/tests/order/test_order.cpp
    software/tests/order/test_order_validation.cpp
)

# Links the test runner to your core engine and the downloaded GoogleTest framework.
# GTest::gtest_main automatically provides a main() function for the tests.
target_link_libraries(cme_tests
    PRIVATE
        cme_core
        GTest::gtest_main
)

# Includes advanced CMake helper functions specifically designed for GoogleTest.
include(GoogleTest)

# Automatically scans your test source files and registers individual tests with CTest.
gtest_discover_tests(cme_tests)

