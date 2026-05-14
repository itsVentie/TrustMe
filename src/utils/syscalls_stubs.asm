.code
NtTerminateProcess proc
    mov r10, rcx
    mov eax, 2Ch ; Номер для Windows 10/11 (может меняться)
    syscall
    ret
NtTerminateProcess endp
end