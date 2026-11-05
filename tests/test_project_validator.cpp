#include "ProjectValidator.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

bool CreateTestDirectory(const fs::path& testPath)
{
    std::error_code ec;

    bool created = fs::create_directory(testPath, ec);

    if (ec || !created)
    {
        std::cerr << "Failed to create test directory: "
            << testPath << '\n';

        if (ec)
        {
            std::cerr << ec.message() << '\n';
        }

        return false;
    }

    return true;
}

bool CreateCmakeFile(const fs::path& testPath)
{
    std::ofstream cmakeFile(testPath / "CMakeLists.txt");
    if (!cmakeFile)
    {
        std::cerr << "Failed to create CMakeLists.txt\n";
        return false;
    }

    return true;
}

bool CreateReadmeFile(const fs::path& testPath)
{
    std::ofstream readmeFile(testPath / "README.md");
    if (!readmeFile)
    {
        std::cerr << "Failed to create README.md\n";
        return false;
    }
    return true;
}

bool CreateSrcDirectory(const fs::path& testPath)
{
    std::error_code ec;
    bool created = fs::create_directory(testPath / "src", ec);
    if (ec || !created)
    {
        std::cerr << "Failed to create src directory\n";

        if (ec)
        {
            std::cerr << ec.message() << '\n';
        }

        return false;
    }
    return true;
}

bool CleanupTestDirectory(const fs::path& testPath)
{
    std::error_code ec;

    fs::remove_all(testPath, ec);

    if (ec)
    {
        std::cerr << "Cleanup failed: "
            << ec.message() << '\n';
        return false;
    }

    return true;
}

bool TestValidProject()
{
    fs::path testPath = "test_workspace_valid";
    if (!CreateTestDirectory(testPath))
    {
        return false;
    }

    bool setupSuccess =
        CreateCmakeFile(testPath) &&
        CreateReadmeFile(testPath) &&
        CreateSrcDirectory(testPath);

    if (!setupSuccess)
    {
		CleanupTestDirectory(testPath);
        return false;
    }

    CheckResult result = CheckProject(testPath);

    bool cleanupSuccess = CleanupTestDirectory(testPath);

    return result.status == CheckStatus::Pass && cleanupSuccess;
}

bool TestMissingReadme()
{
    fs::path testPath = "test_workspace_missing_readme";
    if (!CreateTestDirectory(testPath))
    {
        return false;
    }

    bool setupSuccess =
        CreateCmakeFile(testPath) &&
        CreateSrcDirectory(testPath);

    if (!setupSuccess)
    {
        CleanupTestDirectory(testPath);
        return false;
    }

    CheckResult result = CheckProject(testPath);

    bool hasMissingReadme =
        std::find_if(
            result.diagnostics.begin(),
            result.diagnostics.end(),
            [](const Diagnostic& diagnostic)
            {
                return diagnostic.item == "README.md" &&
                    diagnostic.status == CheckStatus::Fail &&
                    diagnostic.message == "Required file is missing";
            }
        ) != result.diagnostics.end();

    bool cleanupSuccess = CleanupTestDirectory(testPath);

    return result.status == CheckStatus::Fail &&
        cleanupSuccess &&
        hasMissingReadme;
}

bool TestMissingCMake()
{
    fs::path testPath = "test_workspace_missing_cmake";
    if (!CreateTestDirectory(testPath))
    {
        return false;
    }

    bool setupSuccess =
        CreateReadmeFile(testPath) &&
        CreateSrcDirectory(testPath);

    if (!setupSuccess)
    {
        CleanupTestDirectory(testPath);
        return false;
    }

    CheckResult result = CheckProject(testPath);

    bool hasMissingCmake =
        std::find_if(
            result.diagnostics.begin(),
            result.diagnostics.end(),
            [](const Diagnostic& diagnostic)
            {
                return diagnostic.item == "CMakeLists.txt" &&
                    diagnostic.status == CheckStatus::Fail &&
                    diagnostic.message == "Required file is missing";
            }
        ) != result.diagnostics.end();

    bool cleanupSuccess = CleanupTestDirectory(testPath);

    return result.status == CheckStatus::Fail &&
        cleanupSuccess &&
        hasMissingCmake;
}

bool TestMissingSrc()
{
    fs::path testPath = "test_workspace_missing_src";
    if (!CreateTestDirectory(testPath))
    {
        return false;
	}
    bool setupSuccess =
        CreateCmakeFile(testPath) &&
        CreateReadmeFile(testPath);

    if (!setupSuccess)
    {
        CleanupTestDirectory(testPath);
        return false;
    }

    CheckResult result = CheckProject(testPath);

    bool hasMissingSrc =
        std::find_if(
            result.diagnostics.begin(),
            result.diagnostics.end(),
            [](const Diagnostic& diagnostic)
            {
                return diagnostic.item == "src/" &&
                    diagnostic.status == CheckStatus::Fail &&
                    diagnostic.message == "Required directory is missing";
            }
        ) != result.diagnostics.end();

    bool cleanupSuccess = CleanupTestDirectory(testPath);

    return result.status == CheckStatus::Fail &&
        cleanupSuccess &&
        hasMissingSrc;
}

bool TestInvalidPath()
{
    fs::path testPath = "non_existent_path";

    std::error_code ec;
    bool exists = fs::exists(testPath, ec);

    if (ec || exists)
    {
        std::cerr << "Invalid test precondition\n";
        return false;
    }

    CheckResult result = CheckProject(testPath);

    return result.status == CheckStatus::Error;
}

bool TestWrongFileType()
{
    fs::path testPath = "test_workspace_wrong_type";
    if (!CreateTestDirectory(testPath))
    {
        return false;
	}

    std::error_code ec;

    bool created = fs::create_directory(
        testPath / "README.md",
        ec
    );

    bool setupSuccess =
        CreateCmakeFile(testPath) &&
        !ec &&
        created &&
        CreateSrcDirectory(testPath);

    if (!setupSuccess)
    {
        CleanupTestDirectory(testPath);
        return false;
    }

    CheckResult result = CheckProject(testPath);

    bool hasWrongReadme =
        std::find_if(
            result.diagnostics.begin(),
            result.diagnostics.end(),
            [](const Diagnostic& diagnostic)
            {
                return diagnostic.item == "README.md" &&
                    diagnostic.status == CheckStatus::Fail &&
                    diagnostic.message == "Expected a regular file, but found a directory";
            }
        ) != result.diagnostics.end();

    bool cleanupSuccess = CleanupTestDirectory(testPath);

    return result.status == CheckStatus::Fail &&
        cleanupSuccess &&
        hasWrongReadme;
}

bool TestMultipleMissingItems()
{
    fs::path testPath = "test_workspace_multiple_missing";
    if (!CreateTestDirectory(testPath))
    {
        return false;
    }
    bool setupSuccess =
        CreateSrcDirectory(testPath);

    if (!setupSuccess)
    {
        CleanupTestDirectory(testPath);
        return false;
    }
    CheckResult result = CheckProject(testPath);
    bool hasMissingCmake =
        std::find_if(
            result.diagnostics.begin(),
            result.diagnostics.end(),
            [](const Diagnostic& diagnostic)
            {
                return diagnostic.item == "CMakeLists.txt" &&
                    diagnostic.status == CheckStatus::Fail &&
                    diagnostic.message == "Required file is missing";
            }
        ) != result.diagnostics.end();

    bool hasMissingReadme =
        std::find_if(
            result.diagnostics.begin(),
            result.diagnostics.end(),
            [](const Diagnostic& diagnostic)
            {
                return diagnostic.item == "README.md" &&
                    diagnostic.status == CheckStatus::Fail &&
                    diagnostic.message == "Required file is missing";
            }
        ) != result.diagnostics.end();

    bool cleanupSuccess = CleanupTestDirectory(testPath);

    return result.status == CheckStatus::Fail &&
        cleanupSuccess &&
        hasMissingCmake &&
        hasMissingReadme;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: tests <test_name>\n";
        return 2;
    }

    std::string testName = argv[1];

    if (testName == "ValidProject")
        return TestValidProject() ? 0 : 1;

    if (testName == "MissingReadme")
        return TestMissingReadme() ? 0 : 1;

    if (testName == "MissingCMake")
        return TestMissingCMake() ? 0 : 1;

    if (testName == "MissingSrc")
        return TestMissingSrc() ? 0 : 1;

    if (testName == "InvalidPath")
        return TestInvalidPath() ? 0 : 1;

    if (testName == "WrongFileType")
        return TestWrongFileType() ? 0 : 1;

    if (testName == "MultipleMissingItems")
        return TestMultipleMissingItems() ? 0 : 1;

    std::cerr << "Unknown test case\n";
    return 2;
}