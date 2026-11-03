# Week 13 Day 1 ¡X Project Structure Validation

**Date:** 2026-11-02

## Architecture Overview

The Project Health Checker is a C++20 command-line application designed to validate the basic structure of a C++ project.

The application accepts a project directory through command-line arguments (`argc` and `argv`). The `main()` function validates the input directory before passing its path to `CheckProject()`.

The validator uses `std::filesystem` to check whether the required files and directories exist and have the expected types.

The validation result is represented by a `CheckResult` structure containing the validation status, failed items, and filesystem error status.

The CLI uses three exit codes:

- `0` ¡X Validation passed.
- `1` ¡X Project structure validation failed.
- `2` ¡X Invalid arguments or filesystem operation error.

This design allows the tool to communicate results to external automation systems.

## Error Handling

The application distinguishes between missing required files and filesystem operation errors.

A missing file indicates that the project structure does not meet the validation requirements. A filesystem operation error indicates that the application could not complete a filesystem query reliably.

Possible causes include insufficient permissions, invalid filesystem operations, or network filesystem failures.

The application uses `try / catch` to handle `std::filesystem::filesystem_error` exceptions and records filesystem errors separately from validation failures.

This prevents filesystem errors from being incorrectly reported as successful validation.

## CI Pipeline Integration

The CLI is designed to support automated validation workflows.

It provides human-readable diagnostic messages and meaningful exit codes, allowing tools such as Python scripts, CTest, or GitHub Actions to determine whether validation succeeded.

Future improvements will include automated testing and continuous integration.

## What I Learned

This project builds upon my previous experience with CMake, Python automation, and C++ error handling.

I practiced organizing C++ source and header files using CMake, including configuring include directories and executable targets.

My previous Python automation projects used `subprocess.run()` and process return codes to detect execution results. This project helped me understand how to design a C++ CLI that can be reliably consumed by such automation tools.

I also applied C++ exception handling and `std::filesystem` to distinguish project validation failures from filesystem operation errors.

Overall, this project strengthened my understanding of modular application design, command-line interfaces, and reliable error reporting.



# Week 13 Day 2 ¡X Filesystem Testing & Refactoring

**Date:** 2026-11-03

## Testing Architecture

I designed filesystem validation tests using isolated test directories instead of depending on existing project directories.

Each test creates the required files and directories to simulate a specific scenario, such as a valid project or a project with missing required files.

The test environment is cleaned up after execution to avoid leaving temporary files in the repository.

I separated different scenarios into individual test functions, such as `TestValidProject()` and `TestMissingReadme()`, to improve readability, maintainability, and debugging.

This approach also helps reduce dependencies between test cases.

## Debugging Notes

At the beginning, I was unsure how to generate test data and organize reusable testing utilities.

I started by implementing the valid-project test case to understand the basic setup and validation process.

After successfully implementing the first test, I gradually added additional scenarios.

Once the test cases were working, I identified duplicated code and extracted reusable helper functions.

This allowed me to refactor the test implementation without introducing unnecessary complexity at the beginning.

An important follow-up is to rerun all tests after refactoring to ensure that the expected behavior remains unchanged.

## Compilation vs. Correctness

Successful compilation does not guarantee that a program behaves correctly.

A compiler checks whether the code satisfies language and type requirements, while a linker resolves the symbols needed to build the executable.

However, neither process can generally determine whether the application fulfills its intended requirements.

For example, a filesystem validator may compile successfully but return an incorrect result when a required file is missing or has the wrong type.

Automated tests help verify the program's behavior across valid, invalid, and edge-case inputs.

## What I Learned

Today, I practiced designing isolated filesystem tests using temporary directories and test-specific data.

I learned why separating test cases into individual functions improves debugging and maintainability, and why test isolation is necessary for reliable results.

I also practiced incremental development: implementing a working test first, adding more scenarios, and then refactoring duplicated logic into reusable functions.

Most importantly, I reinforced the distinction between successful compilation and correct runtime behavior.

These concepts will support the next stage of integrating automated tests into a CI pipeline.