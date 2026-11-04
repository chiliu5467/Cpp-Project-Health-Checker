
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

# 3. CTest integration
include(CTest)

if(BUILD_TESTING)

    # Test executable
    add_executable(project_validator_tests
        tests/test_project_validator.cpp
    )

    target_link_libraries(project_validator_tests
        PRIVATE project_validator
    )

    # Register test cases
    foreach(test_name IN ITEMS
        ValidProject
        MissingReadme
        MissingCMake
        MissingSrc
        InvalidPath
        WrongFileType
    )
        add_test(
            NAME ${test_name}
            COMMAND
                $<TARGET_FILE:project_validator_tests>
                ${test_name}
        )
    endforeach()

endif()
