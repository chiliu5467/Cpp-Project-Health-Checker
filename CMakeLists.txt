
cmake_minimum_required(VERSION 3.20)

project(CppProjectHealthChecker LANGUAGES CXX)

# 1. Shared validation library
add_library(project_validator STATIC
    src/ProjectValidator.cpp
)

target_compile_features(project_validator
    PUBLIC cxx_std_20
)

target_include_directories(project_validator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 2. Main CLI executable
add_executable(project_health_checker
    src/main.cpp
)

target_link_libraries(project_health_checker
    PRIVATE project_validator
)

# 3. Test executable
add_executable(project_validator_tests
    tests/test_project_validator.cpp
)

target_link_libraries(project_validator_tests
    PRIVATE project_validator
)

# 4. CTest integration
enable_testing()

add_test(
    NAME ValidProject
    COMMAND project_validator_tests ValidProject
)

add_test(
    NAME MissingReadme
    COMMAND project_validator_tests MissingReadme
)

add_test(
    NAME MissingCMake
    COMMAND project_validator_tests MissingCMake
)

add_test(
    NAME MissingSrc
    COMMAND project_validator_tests MissingSrc
)

add_test(
    NAME InvalidPath
    COMMAND project_validator_tests InvalidPath
)

add_test(
    NAME WrongFileType
    COMMAND project_validator_tests WrongFileType
)
