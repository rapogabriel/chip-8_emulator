#include <string.h>

#include "argparser.h"
#include "config.h"
#include "debug.h"

// Estrutura X(NOME_NO_ENUM, FLAG_EM_STRING, NOME_DO_CAMPO_NA_CONFIG, VALOR_QUE_DEVE_SER_SETADO)
#define LISTA_FLAGS(X) \
X(RETRO, "--retro", retro, true) \
X(AMIGA, "--amiga", amiga, true)

#define X(nome_enum, str, campo, valor) nome_enum,
typedef enum _FLAG {
    DESCONHECIDA,
    FILEPATH,
    LISTA_FLAGS(X)
} FLAG;
#undef X

FLAG parseArg(const char argv[]);

#define X(nome_enum, str, campo, valor) \
case nome_enum: if(cfg->campo != valor) cfg->campo = valor; \
else { LOG("Erro de flag repetida: %s", str); return false; } \
break;
bool parse(int argc, char* argv[], Config* cfg){
    if(argc < 2) return false;
    
    for(int i = 1; i < argc; ++i){
        switch (parseArg(argv[i])) {
            case FILEPATH:
                if(cfg->filepath == nullptr) cfg->filepath = argv[i];
                else { LOG("Parser identificou um filepath adicional: %s", argv[i]); return false; }
                break;
            LISTA_FLAGS(X)
            default: LOG("Elemento desconhecido no parser: %s", argv[i]); return false;
        }
    }

    if(cfg->filepath == nullptr) {
        LOG("Parser não identificou o caminho da ROM ao fim do parsing");
        return false;
    }
    return true;
}
#undef X

#define X(nome_enum, str, campo, valor) \
if(!strcmp(argv, str)) return nome_enum;
FLAG parseArg(const char argv[]){
    if(argv[0] != '-') return FILEPATH;

    LISTA_FLAGS(X)
    
    return DESCONHECIDA;
}
#undef X