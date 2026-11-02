cmake_minimum_required(VERSION 3.20)

project(CppProjectHealthChecker LANGUAGES CXX)

add_executable(project_health_checker
    src/main.cpp
    src/ProjectValidator.cpp
)

target_compile_features(project_health_checker
    PRIVATE cxx_std_20
)

target_include_directories(project_health_checker
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)