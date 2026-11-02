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