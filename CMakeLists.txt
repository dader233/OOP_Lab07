cmake_minimum_required(VERSION 3.10)
project(lab_07)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "-O0 -ggdb")
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.0
        TLS_VERIFY false
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Ваша библиотека с NPC
add_library(${CMAKE_PROJECT_NAME}_lib
        src/npc.cpp
        src/orc.cpp
        src/squirrel.cpp
        src/druid.cpp
        src/factory_npc.cpp
        src/visitor_sim_fight.cpp
        src/core.cpp
        src/observer.cpp
)

# Главный исполняемый файл (main.cpp в корне)
add_executable(${CMAKE_PROJECT_NAME}_out main.cpp)

# Связываем с библиотекой
target_link_libraries(${CMAKE_PROJECT_NAME}_out PRIVATE ${CMAKE_PROJECT_NAME}_lib)

# Включаем тестирование
enable_testing()

# Тесты (если нужны)
add_executable(tests tests/test.cpp)
target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib gtest_main)

add_test(NAME MyProjectTests COMMAND tests)