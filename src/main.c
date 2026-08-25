#include <stdio.h>

#include "arg_parse.h"

int main(int argc, char* argv[]){
    printf("Olá mundo!\n");
    // Placeholder
    [[maybe_unused]]
    parseResul ind = arg_parse(argc, argv);
    return 0;
}