set positional-arguments

export CC := env_var_or_default("CC", "clang")

default:
    just --list

# Configure cmake from scratch with mode: debug, release, or o0.
configure mode="debug":
    #!/usr/bin/env sh
    set -eu

    mode='{{ mode }}'

    rm -rf build/

    case "$mode" in
        debug)
            cmake -B build -DCMAKE_BUILD_TYPE=Debug
            ;;
        release)
            cmake -B build -DCMAKE_BUILD_TYPE=Release
            ;;
        o0)
            DISABLE_OPTIMIZATIONS=1 cmake -B build -DCMAKE_BUILD_TYPE=Debug
            ;;
        *)
            printf 'Unknown build mode: %s\nExpected one of: debug, release, o0\n' "$mode" >&2
            exit 1
            ;;
    esac

# Build, configuring debug first if needed.
build:
    if [ ! -d build ]; then just configure; fi
    cmake --build build
    if [ -f build/compile_commands.json ]; then mkdir -p .compile-db && cp build/compile_commands.json .compile-db; fi

# Build and run the app.
run *args: build
    ./build/app/ktest-app "$@"

# Build and run tests.
test *args: build
    ctest --test-dir build/test --output-on-failure --verbose "$@"

# Copy vscode debug/task config into place.
setup-vscode:
    mkdir -p .vscode/
    cp dev/vscode/* .vscode/

# Format source files.
format:
    ./format.sh
