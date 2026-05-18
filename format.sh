#!/usr/bin/env sh
set -eu

cd "$(dirname "$(readlink -f -- "$0")")"

mode="${1:-all}"

case "$mode" in
    all)
        files=$(git ls-files)
        ;;
    diff)
        files=$(
            {
                git diff --name-only --diff-filter=ACMR
                git diff --cached --name-only --diff-filter=ACMR
            } | sort -u
        )
        ;;
    *)
        printf 'Unknown format mode: %s\nExpected one of: all, diff\n' "$mode" >&2
        exit 1
        ;;
esac

printf '%s\n' "$files" | while IFS= read -r file; do
    if [ ! -f "$file" ]; then
        continue
    fi

    case "$file" in
        *.c | *.h)
            clang-format -i "$file"
            ;;
        CMakeLists.txt | *.cmake)
            cmake-format -i "$file"
            ;;
        *.nix)
            nix fmt "$file"
            ;;
    esac
done
