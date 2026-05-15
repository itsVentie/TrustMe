#pragma once
#include <windows.h>
#include <string>

inline std::string XorString(const char* data) {
    std::string output = data;
    char key = 0x7F; 
    for (size_t i = 0; i < output.size(); i++) output[i] ^= key;
    return output;
}

#undef X
#define X(str) XorString(str).c_str()

constexpr DWORD HashAPI(const char* str) {
    DWORD hash = 0x811c9dc5;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 0x01000193;
    }
    return hash;
}

void ComplexJunk();
FARPROC GetAPI(HMODULE hMod, DWORD hsh);

#define JUNK_CODE_BLOCK \
    do { \
        volatile unsigned long long _val = __LINE__ * 0xDEADBEEF; \
        for(int _i=0; _i<5; _i++) { _val ^= (_i + 0x1337); _val *= 2; } \
    } while(0)