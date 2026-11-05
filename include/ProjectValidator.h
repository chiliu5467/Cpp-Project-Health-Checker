#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

enum class CheckStatus
{
    Pass,
    Fail,
    Error
};

struct Diagnostic
{
    CheckStatus status;
    std::string item;
    std::string message;
};

struct CheckResult
{
    CheckStatus status = CheckStatus::Pass;
    std::vector<Diagnostic> diagnostics;
};

CheckResult CheckProject(
    const std::filesystem::path& projectPath
);
