# CHIP-8 Emulator

Um emulador da máquina virtual CHIP-8 desenvolvido puramente em **C (padrão C23)**. 

No momento, o projeto encontra-se em sua fase inicial de desenvolvimento (exibindo apenas um *Hello World* estrutural). O objetivo principal deste repositório é servir como um ambiente prático de estudo aprofundado em arquitetura de computadores, manipulação de memória e engenharia de sistemas de baixo nível.

## 🗂️ Estrutura do Projeto

```text
.
├── include/
│   └── arg_parse.h       # Declarações e estruturas para o parser de argumentos da CLI
├── src/
│   ├── arg_parse.c       # Implementação da lógica de tratamento da linha de comando
│   └── main.c            # Ponto de entrada do emulador (atualmente Hello World)
├── .gitignore            # Regras de exclusão para o repositório
├── CMakeLists.txt        # Configurações centrais de compilação
├── CMakePresets.json     # Predefinições de ambiente e build do CMake
└── justfile              # Receitas de automação para build e execução
```

## 🛠️ Ferramentas e Pré-requisitos

O projeto utilizou das seguintes ferramentas:

*   **Clang:** 22.1.8
*   **CMake:** 4.4.2
*   **Ninja:** 1.13.2
*   **Just:** 1.58.0

Para compilar e executar localmente, certifique-se de tê-las instaladas no seu sistema.

## 🚀 Compilação e Execução

O fluxo de trabalho foi totalmente abstraído pelo `justfile`, eliminando a necessidade de invocar ferramentas de compilação manualmente no dia a dia.

### Comandos Disponíveis

*   **`just run`**: Compila e roda o executável a partir da pasta de build.
    *   *Comportamento padrão:* Caso a pasta de build não exista, este comando irá gerar automaticamente uma build configurada para **debug**.
    *   *Flag opcional:* Você pode utilizar **`just run --nodebug`**. Nesse caso, se a pasta de build não existir, ele irá gerar uma build **sem debug** (ideal para performance final).
*   **`just clean`**: Apaga a pasta de build.

## 🗺️ Próximos Passos (Roadmap)

Como o projeto está no início, o desenvolvimento seguirá uma abordagem orientada à experiência visual e interativa. As próximas etapas envolverão:

- [ ] **Interface Gráfica Base (SDL3):** O desenvolvimento da UI será a prioridade inicial, antes da implementação da lógica do emulador em si. Isso incluirá a janela base, botões para carregar a ROM e opções para fechar o programa, criando uma "carcaça" funcional para o emulador.
- [ ] Implementação da CPU e do ciclo de *Fetch-Decode-Execute*.
- [ ] Mapeamento da memória e configuração dos registradores.
- [ ] Integração do backend do emulador com a interface SDL3 para renderização do display do CHIP-8 e captura de inputs