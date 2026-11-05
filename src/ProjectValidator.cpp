#include "ProjectValidator.h"

CheckResult CheckProject(
    const std::filesystem::path& projectPath)
{
    CheckResult result{};
    std::error_code ec;

    bool isDirectory = fs::is_directory(projectPath, ec);

    if (ec)
    {
        result.diagnostics.push_back({
            CheckStatus::Error,
            projectPath.string(),
            ec.message()
            });

        result.status = CheckStatus::Error;
        return result;
    }

    if (!isDirectory)
    {
        result.diagnostics.push_back({
            CheckStatus::Error,
            projectPath.string(),
            "Invalid project directory"
            });

        result.status = CheckStatus::Error;
        return result;
    }

    // Step 1: 檢查 CMakeLists.txt
    fs::path path = projectPath / "CMakeLists.txt";

    try
    {
        if (fs::is_regular_file(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Pass,
                "CMakeLists.txt",
                "Valid regular file"
                });
        }
        else if (fs::is_directory(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "CMakeLists.txt",
                "Expected a regular file, but found a directory"
                });
        }
        else if (!fs::exists(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "CMakeLists.txt",
                "Required file is missing"
                });
        }
        else
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "CMakeLists.txt",
                "Required file has an unsupported file type"
                });
        }
    }
    catch (const fs::filesystem_error& e)
    {
        result.diagnostics.push_back({
            CheckStatus::Error,
            "CMakeLists.txt",
            e.what()
            });

        result.status = CheckStatus::Error;
    }

    // Step 2: 檢查 README.md
    path = projectPath / "README.md";

    try
    {
        if (fs::is_regular_file(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Pass,
                "README.md",
                "Valid regular file"
                });
        }
        else if (fs::is_directory(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "README.md",
                "Expected a regular file, but found a directory"
                });
        }
        else if (!fs::exists(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "README.md",
                "Required file is missing"
                });
        }
        else
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "README.md",
                "Required file has an unsupported file type"
                });
        }
    }
    catch (const fs::filesystem_error& e)
    {
        result.diagnostics.push_back({
            CheckStatus::Error,
            "README.md",
            e.what()
            });

        result.status = CheckStatus::Error;
    }

    // Step 3: 檢查 src/ 資料夾
	path = projectPath / "src";
    try
    {
        if (fs::is_regular_file(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "src/",
                "Expected a directory, but found a regular file"
                });
        }
        else if (fs::is_directory(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Pass,
                "src/",
                "Valid directory"
                });
        }
        else if (!fs::exists(path))
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "src/",
                "Required directory is missing"
                });
        }
        else
        {
            result.diagnostics.push_back({
                CheckStatus::Fail,
                "src/",
                "Expected a directory, but found an unsupported file type"
                });
        }
    }
    catch (const fs::filesystem_error& e)
    {
        result.diagnostics.push_back({
            CheckStatus::Error,
            "src/",
            e.what()
            });

        result.status = CheckStatus::Error;
    }

    // Step 4: 設定 success
    result.status = CheckStatus::Pass;

    for (const auto& diagnostic : result.diagnostics)
    {
        if (diagnostic.status == CheckStatus::Error)
        {
            result.status = CheckStatus::Error;
            break;
        }

        if (diagnostic.status == CheckStatus::Fail)
        {
            result.status = CheckStatus::Fail;
        }
    }

    return result;
}