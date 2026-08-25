set quiet := true

[arg('nodebug', long, value= 'true')]
run nodebug='false': (_verifica_init nodebug)
    ninja -C build run
_init nodebug:
    @{{ if nodebug == "false" { "cmake --preset default -D ENABLE_EMU_DEBUG=true" } else { "cmake --preset default" } }}

[linux]
[macos]
_verifica_init nodebug:
    if [ ! -d "build" ]; then just _init {{nodebug}}; fi

[windows]
_verifica_init nodebug:
    powershell -Command "if (-Not (Test-Path 'build')) { just _init {{nodebug}} }"

[linux]
[macos]
clean:
    rm -rf build
[windows]
clean:
    powershell -Command "Remove-Item '.\build' -Recurse -Force -ErrorAction SilentlyContinue"
