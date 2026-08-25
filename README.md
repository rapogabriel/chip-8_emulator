# CHIP-8 Emulator

Um emulador da máquina virtual CHIP-8 desenvolvido puramente em **C (padrão C23)**. 

No momento, o projeto encontra-se em sua fase inicial de desenvolvimento. O objetivo principal deste repositório é servir como um ambiente prático de estudo aprofundado em arquitetura de computadores, manipulação de memória e engenharia de sistemas de baixo nível.

## 🗂️ Estrutura do Projeto

```text
.
├── include/
│   └── arg_parse.h       # Declarações e estruturas para o parser de argumentos da CLI
├── src/
│   ├── arg_parse.c       # Implementação da lógica de tratamento da linha de comando
│   └── main.c            # Ponto de entrada do emulador (atualmente Hello World)
├── .dockerignore         # Regras de exclusão para o Docker
├── .gitignore            # Regras de exclusão para o repositório
├── CMakeLists.txt        # Configurações centrais de compilação
├── CMakePresets.json     # Predefinições de ambiente e build do CMake
├── Dockerfile            # Configuração da imagem com o toolchain para ambiente isolado
└── justfile              # Receitas de automação para build e execução
```

## 🛠️ Ferramentas e Pré-requisitos

Você pode executar este projeto utilizando **Docker** (para evitar configurar o ambiente localmente) ou instalando todas as dependências nativamente.

### Opção A: Usando Docker (Recomendado)
Para rodar de forma isolada, você precisará de:
*   **Docker** (No Windows, requer Docker Desktop configurado para usar a engine do WSL2 para suporte ao WSLg).
*   **Just**: Instalado na sua máquina apenas para encurtar os comandos do Docker.

### Opção B: Instalação Local
O projeto utilizou das seguintes ferramentas:

*   **Clang:** 22.1.8
*   **CMake:** 4.4.2
*   **Ninja:** 1.13.2
*   **Just:** 1.58.0

Para compilar e executar localmente sem contêineres, certifique-se de ter essas ferramentas no seu sistema.

## 🚀 Compilação e Execução

O fluxo de trabalho foi abstraído pelo `justfile`. 

> ⚠️ Ao alternar entre a compilação local e a compilação via Docker, lembre-se de rodar o comando de limpeza (`just clean` localmente ou apagando a pasta `build`). Isso evita que caminhos absolutos no cache do CMake gerem falhas de compilação.

### Executando via Docker

O processo de build ocorre por partes. Isso garante que as alterações que você faz no código sejam refletidas na hora pelo contêiner, sem lentidão ou necessidade de reconstruir a imagem a cada modificação.

**Se você possui o `just` instalado na sua máquina**, utilize os atalhos:
*   **`just docker-build`**: Constrói a imagem Docker local contendo todo o toolchain (execute apenas uma vez).
*   **`just docker-run`**: Sobe o contêiner, compila o código e abre a janela do emulador compartilhando o display nativo (suporta X11 no Linux e WSLg no Windows).

**Se você possui APENAS o Docker instalado**, utilize os comandos abaixo:

1. Construa a imagem (apenas na primeira vez):
```bash
docker build -t chip8-emu .
```

2. Execute o emulador compartilhando o display nativo:
*   **No Linux:**
    ```bash
    docker run --rm -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v "$PWD":/app chip8-emu just run
    ```
*   **No Windows (PowerShell):**
    ```powershell
    wsl bash -c "docker run --rm -it -e DISPLAY=:0 -v /tmp/.X11-unix:/tmp/.X11-unix -v \$PWD:/app chip8-emu just run"
    ```

### Executando Localmente

*   **`just run`**: Compila e roda o executável a partir da pasta de build.
    *   *Comportamento padrão:* Caso a pasta de build não exista, gerará automaticamente uma build configurada para **debug**.
    *   *Flag opcional:* **`just run --nodebug`** gera uma build **sem debug** caso a pasta não exista.
*   **`just clean`**: Apaga a pasta de build.

## 🗺️ Próximos Passos (Roadmap)

- [ ] **Interface Gráfica Base (SDL3):** O desenvolvimento da UI será a prioridade inicial, antes da implementação da lógica do emulador em si. Isso incluirá a janela base, botões para carregar a ROM e opções para fechar o programa, criando uma carcaça funcional para o emulador.
- [ ] Implementação da CPU e do ciclo de *Fetch-Decode-Execute*.
- [ ] Mapeamento da memória e configuração dos registradores.
- [ ] Integração do backend do emulador com a interface SDL3 para renderização do display do CHIP-8 e captura de inputs.