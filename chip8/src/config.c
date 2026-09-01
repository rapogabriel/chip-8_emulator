#include "string.h"

#include "config.h"

Config setConfig(){
    Config cfg;
    memset(&cfg, 0, sizeof(cfg));
    return cfg;
}