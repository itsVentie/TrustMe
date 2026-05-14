#include <windows.h>
#include "../../include/utils/logger.h"
#include <fstream>
#include <chrono>
#include <iomanip>

void Log(const std::string& message) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        logFile << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << " - " << message << std::endl;
        logFile.close();
    }
}

void SelfDestruct() {
    char path[MAX_PATH];
    char cmd[MAX_PATH + 70];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    sprintf(cmd, "cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"", path);
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if(CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    exit(0);
}