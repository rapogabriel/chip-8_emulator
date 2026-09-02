# Versão mínima do CMake
cmake_minimum_required(VERSION 3.28)

# Nome do projeto e versão padrão do C++
project(EMULADOR_CHIP_8 C)
set(CMAKE_C_STANDARD 23)

# Define os diretórios
include_directories(include)
set(SRC_DIR src/)
set(EXECUTAVEL chip8)
set(FONTES
    main
    core
    render
    btn_actions
    chip-8
)

# Define que a versão do C++ é obrigatória e desativa extensões específicas do compilador
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Habilita o FetchContent
include(FetchContent)

set(SDL_STATIC ON CACHE BOOL "Força a compilação estática do SDL3" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Desliga a compilação dinâmica" FORCE)

# Diz ao CMake onde buscar o SDL2
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.12 # Use a tag da versão mais recente
)

# Baixa e deixa disponível
FetchContent_MakeAvailable(SDL3)

option(ENABLE_EMU_DEBUG "Ativa a flag DEBUG para logs" OFF)

# Transforma um arquivo em FONTES para o padrão ${SRC_DIR}arquivo.c
list(TRANSFORM FONTES PREPEND ${SRC_DIR})
list(TRANSFORM FONTES APPEND .c)

# Permite a exportação dos comandos de compilação para o clangd funcionar direito
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define o executavel
add_executable(${EXECUTAVEL} ${FONTES})

# target_link_libraries(${EXECUTAVEL} PRIVATE SDL3::SDL3)

target_compile_options(${EXECUTAVEL} PRIVATE 
-Wall -Wextra -Werror -Wpedantic -Wshadow 
-Wconversion -Wsign-conversion)

target_link_libraries(${EXECUTAVEL} PRIVATE SDL3::SDL3)

if(ENABLE_EMU_DEBUG)
    # #ifdef DEBUG
    target_compile_definitions(${EXECUTAVEL} PRIVATE DEBUG)
    
    # Um aviso visual no terminal
    message(STATUS "MODO DEBUG ATIVADO: Logs ligados.")
else()
    message(STATUS "MODO RELEASE: Compilação padrão (sem logs de debug).")
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:${EXECUTAVEL}>
    DEPENDS ${EXECUTAVEL}
)