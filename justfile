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

# Build and run tests.
test *args: build
    ctest --test-dir build/test --output-on-failure --verbose "$@"

# Copy vscode debug/task config into place.
setup-vscode:
    mkdir -p .vscode/
    cp dev/vscode/* .vscode/

# Format source files. Use `just format diff` for git-changed files only.
format mode="all":
    ./format.sh "{{ mode }}"

# Install a pre-commit hook that formats git-changed files.
git-hooks:
    #!/usr/bin/env sh
    set -eu

    hook=.git/hooks/pre-commit

    if [ -e "$hook" ]; then
        printf '%s already exists; not overwriting\n' "$hook" >&2
        exit 1
    fi

    mkdir -p .git/hooks
    cat > "$hook" <<'EOF'
    #!/usr/bin/env sh
    set -eu

    staged_files=$(git diff --cached --name-only --diff-filter=ACMR)

    just format diff

    if [ -n "$staged_files" ] && ! git diff --quiet -- $staged_files; then
        printf 'pre-commit formatted files. Review and stage the changes, then commit again.\n' >&2
        exit 1
    fi
    EOF
    chmod +x "$hook"
