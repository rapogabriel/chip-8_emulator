#pragma once

typedef struct Config {
    bool retro;
    bool amiga;
    const char* filepath;
} Config;

Config setConfig();