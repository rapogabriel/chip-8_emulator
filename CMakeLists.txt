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
    arg_parse
)

# Define que a versão do C++ é obrigatória e desativa extensões específicas do compilador
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(ENABLE_EMU_DEBUG "Ativa a flag DEBUG para logs" OFF)

# Transforma um arquivo em FONTES para o padrão ${SRC_DIR}arquivo.cpp
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

if(ENABLE_EMU_DEBUG)
    # Equivalente a passar -DDEBUG (ativa o #ifdef DEBUG do seu log.h)
    target_compile_definitions(${EXECUTAVEL} PRIVATE DEBUG)
    
    # Injeta as flags do Valgrind
    target_compile_options(${EXECUTAVEL} PRIVATE -g -fno-omit-frame-pointer -O1 -fsanitize=address,undefined)
    target_link_options(${EXECUTAVEL} PRIVATE -fsanitize=address,undefined)
    
    # Um aviso visual no terminal para você ter certeza que ativou
    message(STATUS "MODO DEBUG ATIVADO: Logs ligados.")
else()
    message(STATUS "MODO RELEASE: Compilação padrão (sem logs de debug).")
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:${EXECUTAVEL}>
    DEPENDS ${EXECUTAVEL}
)