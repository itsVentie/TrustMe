#pragma once
#include <windows.h>

extern "C" NTSTATUS NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus);
