# Utilizando uma imagem base rolling-release ou instável (como Debian Sid ou Arch) 
# para garantir acesso às versões mais recentes do Clang (22+), CMake (4.4+) e Just (1.58+)
FROM debian:sid-slim

# Atualiza os repositórios e instala o toolchain e as dependências
RUN apt-get update && apt-get install -y \
    clang \
    cmake \
    ninja-build \
    just \
    libsdl3-dev \
    && rm -rf /var/lib/apt/lists/*

# Define o diretório de trabalho dentro do container
WORKDIR /app