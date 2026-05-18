# ktest

My rather opinionated C project template.

To start a project with this template, run:
```
./init-template.sh new_project_name
```

Don't forget to change the [LICENSE](https://choosealicense.com/).

## Development

Update dependencies
```
nix flake update
```

Requires CMake and a C11 compiler. `just` is optional for development shortcuts. A nix dev shell is available:
```
nix develop
```

Standard build
```
cmake -B build
cmake --build build
```

Test runtime options are configured with environment variables:
```
KTEST_NAME=t_example KTEST_VERBOSE=1 ./build/test/t_smoke
KTEST_LIST=1 ./build/test/t_smoke
KTEST_STOP_ON_FAIL=1 ctest --test-dir build/test
```

Flag variables are enabled by any non-empty value except `0` or `false`.

Useful development commands
```
# Reconfigure cmake
just configure

# Build and test
just test

# Format all files or only git-changed files
just format
just format diff

# Configure release or unoptimized debug
just configure release
just configure o0

# Install the local pre-commit formatting hook
just git-hooks

# Setup vscode debugging
just setup-vscode
```

### Debugging

Example [`launch.json`](dev/vscode/launch.json) and 
[`tasks.json`](dev/vscode/tasks.json) files are included to debug in vscode.

Nix home-manager setup
```nix
    programs.vscode = {
      enable = true;
      package = pkgs.vscodium;
      profiles.default.extensions = with pkgs.vscode-extensions; [
        llvm-vs-code-extensions.lldb-dap
        llvm-vs-code-extensions.vscode-clangd
      ];
    };
```
