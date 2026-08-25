

#include "arg_parse.h"

parseResul arg_parse(int argc, char* argv[]){
    // Placeholder
    if(argc || argv)
        return (parseResul){true};

    return (parseResul){false};
}