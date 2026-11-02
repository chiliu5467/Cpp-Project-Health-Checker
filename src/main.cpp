#include "ProjectValidator.h"
#include <iostream>

int main(int argc, char* argv[])
{
    // Step 1: 確認使用者有提供路徑
    if (argc != 2)
    {
        std::cerr << "Usage: project_health_checker <project_path>\n";
        return 2;
    }

    // Step 2: 取得使用者指定的專案路徑
    fs::path projectPath = argv[1];

    // Step 3: 確認路徑存在且為資料夾
    try
    {
        if (!fs::is_directory(projectPath))
        {
            std::cerr << "[ERROR] Invalid project directory\n";
            return 2;
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "[ERROR] " << e.what() << '\n';
        return 2;
    }

    CheckResult result = CheckProject(projectPath);

    if (result.filesystemError)
    {
        return 2;
    }

    if (!result.success)
    {
        return 1;
    }

    return 0;
}