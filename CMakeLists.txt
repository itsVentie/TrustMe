cmake_minimum_required(VERSION 3.10)

project(TrustMe)

enable_language(ASM_MASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.asm")

add_executable(TrustMe WIN32 
    ${SOURCES}
    ${RESOURCES}
)

target_link_libraries(TrustMe gdi32 user32 advapi32 wininet)

set_target_properties(TrustMe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")