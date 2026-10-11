cmake_minimum_required(VERSION 3.10)
project(hospital_system CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

if(MSVC)
    add_compile_options(/W3 /utf-8)
endif()

enable_testing()

# core/ 全是头文件（模板实现），为给 IDE 展示组织结构声明一个 interface 库
add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_SOURCE_DIR}/src)

# 测试目标
set(TESTS
    test_linkedlist
    test_hashmap
    test_queue
    test_graph
    test_bipartite
)

foreach(T ${TESTS})
    add_executable(${T} tests/${T}.cpp)
    target_link_libraries(${T} PRIVATE core)
    add_test(NAME ${T} COMMAND ${T})
endforeach()
