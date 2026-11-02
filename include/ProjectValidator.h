#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

struct CheckResult
{
    bool success = false;
    bool filesystemError = false;
    std::vector<std::string> failedItems;
};

CheckResult CheckProject(
    const std::filesystem::path& projectPath
);
