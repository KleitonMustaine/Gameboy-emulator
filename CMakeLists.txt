cmake_minimum_required(VERSION 3.20)

project(gbemu
    VERSION 0.1.0
    DESCRIPTION "Game Boy emulator"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Gera compile_commands.json para o clangd / IntelliSense do VS Code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# CONFIGURE_DEPENDS faz o CMake reconfigurar sozinho quando um .c novo aparece
file(GLOB EMU_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c")

add_executable(gbemu
    main.c
    ${EMU_SOURCES}
)

target_include_directories(gbemu PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# SDL2main precisa vir antes de SDL2: ele fornece o WinMain que chama o seu main
target_link_libraries(gbemu PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(gbemu PRIVATE -Wall)
endif()
