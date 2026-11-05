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

    // Step 3: 執行專案驗證
    CheckResult result = CheckProject(projectPath);

    // Step 4: 輸出每一筆 Diagnostic
    for (const auto& diagnostic : result.diagnostics)
    {
        // 根據狀態選擇輸出串流
        std::ostream& output =
            diagnostic.status == CheckStatus::Error
            ? std::cerr
            : std::cout;

        // 根據狀態選擇輸出標籤
        switch (diagnostic.status)
        {
        case CheckStatus::Pass:
            output << "[PASS] ";
            break;

        case CheckStatus::Fail:
            output << "[FAIL] ";
            break;

        case CheckStatus::Error:
            output << "[ERROR] ";
            break;
        }

        // 輸出檢查項目與詳細訊息
        output << diagnostic.item
            << ": " << diagnostic.message << '\n';
    }

    // Step 5: 根據最終驗證狀態回傳 Exit Code
    switch (result.status)
    {
    case CheckStatus::Pass:
        return 0;

    case CheckStatus::Fail:
        return 1;

    case CheckStatus::Error:
        return 2;
    }

    return 2;
}