#include "utils/obfuscation.h"
#include <math.h>

const unsigned char BIG_JUNK[5000000] = { 0x4D, 0x5A, 0x90 }; 

void ComplexJunk() {
    volatile double x = 0.0;
    for (int i = 0; i < 10000; i++) {
        x += sin((double)i) * cos((double)i);
        if (x > 1000.0) x = 0.0;
    }
    
    int junk_array[100];
    for(int j=0; j<100; j++) {
        junk_array[j] = j ^ 0x55;
    }
}

FARPROC GetAPI(HMODULE hMod, DWORD hsh) {
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)hMod;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)hMod + dos->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY exp = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hMod + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    
    DWORD* names = (DWORD*)((BYTE*)hMod + exp->AddressOfNames);
    WORD* ords = (WORD*)((BYTE*)hMod + exp->AddressOfNameOrdinals);
    DWORD* acts = (DWORD*)((BYTE*)hMod + exp->AddressOfFunctions);

    for (DWORD i = 0; i < exp->NumberOfNames; i++) {
        if (HashAPI((const char*)hMod + names[i]) == hsh) {
            return (FARPROC)((BYTE*)hMod + acts[ords[i]]);
        }
    }
    return nullptr;
}