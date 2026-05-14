⚠️ LEGAL DISCLAIMER
This project is for educational and ethical security research purposes only.
The code provided in this repository is intended to demonstrate low-level Windows internals, API hooking bypasses, and obfuscation techniques.

The author assumes no liability and is not responsible for any misuse or damage caused by this program. Usage of this tool for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state, and federal laws.

Project Overview
TrustMe is a technical Proof-of-Concept (PoC) exploring modern evasion methodologies used in Red Teaming operations. The project focuses on bypassing static and dynamic analysis through architectural shifts rather than simple signature modification.

Key Technical Features
Custom API Hashing: Implements FNV-1a hashing to resolve Windows API addresses at runtime, effectively stripping the Import Address Table (IAT) of suspicious strings.
Indirect Syscalls: Utilizes assembly stubs to execute system calls directly, bypassing user-mode EDR/AV hooks in ntdll.dll and kernel32.dll.
Polymorphic Junk Code: Employs pre-processor macros to inject unique, non-functional instruction sets into every build, altering the file's entropy and signature.
Anti-Analysis & Persistence: Includes basic environment checks (Sandbox/VM detection) and legitimate persistence mechanisms via Task Scheduler.

Build Requirements
Compiler: MSVC (Visual Studio 2022)
Build System: CMake 3.20+
Language: C++20 / x64 Assembly (MASM)

Configuration: Release x64 is mandatory to ensure all obfuscation and optimization routines are correctly applied.
