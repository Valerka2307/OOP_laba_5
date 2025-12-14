cmake_minimum_required(VERSION 3.10)
project(OOP_Laba_5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")
endif()

# Основная библиотека проекта
add_library(${PROJECT_NAME}_lib
    src/MemoryResource.cpp
)

target_include_directories(${PROJECT_NAME}_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

# Основной исполняемый файл
add_executable(${PROJECT_NAME}_exe
    main.cpp
)

target_link_libraries(${PROJECT_NAME}_exe
    PRIVATE ${PROJECT_NAME}_lib
)

# Google Test для тестирования
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Тесты для выделения памяти
add_executable(allocation_tests
    test/Allocation_tests.cpp
)

target_link_libraries(allocation_tests
    PRIVATE ${PROJECT_NAME}_lib
    PRIVATE gtest_main
)

target_include_directories(allocation_tests
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

add_test(NAME AllocationTests COMMAND allocation_tests)

# Тесты для итератора
add_executable(iterator_tests
    test/Iterator_tests.cpp
)

target_link_libraries(iterator_tests
    PRIVATE ${PROJECT_NAME}_lib
    PRIVATE gtest_main
)

target_include_directories(iterator_tests
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

add_test(NAME IteratorTests COMMAND iterator_tests)
