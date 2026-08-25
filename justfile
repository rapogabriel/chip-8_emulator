set quiet := true

# =============================================================================
# COMANDOS PRINCIPAIS
# =============================================================================

# Compila e roda o executável a partir da pasta de build.
# Use `--nodebug` para gerar uma build sem símbolos de debug (se a pasta não existir).
[arg('nodebug', long, value= 'true')]
run nodebug='false': (_verifica_init nodebug)
    echo "▶ Iniciando compilação e execução..."
    ninja -C build run

# Apaga a pasta de build, limpando todos os artefatos de compilação.
[linux]
[macos]
clean:
    echo "🧹 Limpando o diretório de build..."
    rm -rf build

[windows]
clean:
    echo "🧹 Limpando o diretório de build..."
    powershell -Command "Remove-Item '.\build' -Recurse -Force -ErrorAction SilentlyContinue"


# =============================================================================
# COMANDOS DOCKER (Ambiente Isolado)
# =============================================================================

# Constrói a imagem Docker local contendo todo o toolchain necessário.
docker-build:
    echo "🐳 Construindo a imagem Docker 'chip8-emu'..."
    docker build -t chip8-emu .

# Roda o emulador dentro do Docker compartilhando o display nativo (Linux).
[linux]
docker-run:
    echo "🐳 Executando ambiente de desenvolvimento isolado..."
    docker run --rm -it \
        -e DISPLAY=$DISPLAY \
        -v /tmp/.X11-unix:/tmp/.X11-unix \
        -v "$PWD":/app \
        chip8-emu just run

# Roda o emulador dentro do Docker usando o servidor gráfico nativo do Windows (WSLg)
# Roda o emulador dentro do Docker usando o servidor gráfico nativo do Windows (WSLg)
[windows]
docker-run:
    echo "🐳 Executando container via WSLg (Nativo do Windows)..."
    powershell -Command 'wsl bash -c "docker run --rm -it -e DISPLAY=:0 -v /tmp/.X11-unix:/tmp/.X11-unix -v \$PWD:/app chip8-emu just run"'


# =============================================================================
# FUNÇÕES INTERNAS (Ocultas do menu principal)
# =============================================================================

# Gera os arquivos do CMake baseados na flag de debug passada.
[private]
_init nodebug:
    echo "⚙️ Configurando CMake (nodebug={{nodebug}})..."
    @{{ if nodebug == "false" { "cmake --preset default -D ENABLE_EMU_DEBUG=true" } else { "cmake --preset default" } }}

# Gatilho condicional: Roda o init apenas se a pasta de build não existir (Linux/macOS).
[private]
[linux]
[macos]
_verifica_init nodebug:
    if [ ! -d "build" ]; then just _init {{nodebug}}; fi

# Gatilho condicional: Roda o init apenas se a pasta de build não existir (Windows).
[private]
[windows]
_verifica_init nodebug:
    powershell -Command "if (-Not (Test-Path 'build')) { just _init {{nodebug}} }"