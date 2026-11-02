#include "ProjectValidator.h"
#include <iostream>

CheckResult CheckProject(
    const std::filesystem::path& projectPath)
{
    CheckResult result{};

    // Step 1: 檢查 CMakeLists.txt
    fs::path path = projectPath / "CMakeLists.txt";

    try
    {
        if (fs::is_regular_file(path))
        {
            std::cout << "[PASS] CMakeLists.txt\n";
        }
        else if (fs::is_directory(path))
        {
            std::cout << "[FAIL] CMakeLists.txt is a folder\n";
            result.failedItems.push_back("CMakeLists.txt");
        }
        else if (!fs::exists(path))
        {
            std::cout << "[FAIL] CMakeLists.txt is missing\n";
			result.failedItems.push_back("CMakeLists.txt");
        }
        else
        {
            std::cout << "路徑存在，但屬於其他類型\n";
            result.failedItems.push_back("CMakeLists.txt");
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "[ERROR] " << e.what() << '\n';
        result.filesystemError = true;
    }

    // Step 2: 檢查 README.md
    path = projectPath / "README.md";

    try
    {
        if (fs::is_regular_file(path))
        {
            std::cout << "[PASS] README.md\n";
        }
        else if (fs::is_directory(path))
        {
            std::cout << "[FAIL] README.md is a folder\n";
            result.failedItems.push_back("README.md");
        }
        else if (!fs::exists(path))
        {
            std::cout << "[FAIL] README.md is missing\n";
            result.failedItems.push_back("README.md");
        }
        else
        {
            std::cout << "路徑存在，但屬於其他類型\n";
            result.failedItems.push_back("README.md");
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "[ERROR] " << e.what() << '\n';
        result.filesystemError = true;
    }

    // Step 3: 檢查 src/ 資料夾
	path = projectPath / "src";
    try
    {
        if (fs::is_regular_file(path))
        {
            std::cout << "[FAIL] src is a regular file\n";
            result.failedItems.push_back("src/");
        }
        else if (fs::is_directory(path))
        {
            std::cout << "[PASS] src/\n";
        }
        else if (!fs::exists(path))
        {
            std::cout << "[FAIL] src is missing\n";
            result.failedItems.push_back("src/");
        }
        else
        {
            std::cout << "路徑存在，但屬於其他類型\n";
            result.failedItems.push_back("src/");
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "[ERROR] " << e.what() << '\n';
        result.filesystemError = true;
    }

    // Step 4: 設定 success
    result.success =
        result.failedItems.empty() &&
        !result.filesystemError;

    return result;
}