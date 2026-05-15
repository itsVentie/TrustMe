#pragma once
#include <windows.h>

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

typedef LONG NTSTATUS;

extern "C" {
    NTSTATUS SysNtTerminateProcess(
        HANDLE ProcessHandle, 
        NTSTATUS ExitStatus
    );

    NTSTATUS SysNtAllocateVirtualMemory(
        HANDLE ProcessHandle, 
        PVOID* BaseAddress, 
        ULONG_PTR ZeroBits, 
        PSIZE_T RegionSize, 
        ULONG AllocationType, 
        ULONG Protect
    );

    NTSTATUS SysNtWriteVirtualMemory(
        HANDLE ProcessHandle, 
        PVOID BaseAddress, 
        PVOID Buffer, 
        SIZE_T NumberOfBytesToWrite, 
        PSIZE_T NumberOfBytesWritten
    );
}