.code

SysNtTerminateProcess proc
    mov r10, rcx
    mov eax, 2Ch      ; NtTerminateProcess SSN (Win 10/11)
    syscall
    ret
SysNtTerminateProcess endp

SysNtAllocateVirtualMemory proc
    mov r10, rcx
    mov eax, 18h      ; NtAllocateVirtualMemory SSN
    syscall
    ret
SysNtAllocateVirtualMemory endp

SysNtWriteVirtualMemory proc
    mov r10, rcx
    mov eax, 3Ah      ; NtWriteVirtualMemory SSN
    syscall
    ret
SysNtWriteVirtualMemory endp

SysNtProtectVirtualMemory proc
    mov r10, rcx
    mov eax, 50h      ; NtProtectVirtualMemory SSN
    syscall
    ret
SysNtProtectVirtualMemory endp

end