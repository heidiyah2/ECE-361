#!/bin/sh

cd "$(dirname "$0")" || exit 1

# Build the two versions of the depth test.
cc -std=c11 -Wall -Wextra -O0 -o tests/depth-o0 tests/depth.c || exit 1
cc -std=c11 -Wall -Wextra -O2 -o tests/depth-o2 tests/depth.c || exit 1

run_depth()
{
    "$1" "$2" "$3" > /dev/null 2>&1 &
    process=$!
    wait "$process" 2>/dev/null
}

find_max_n()
{
    program=$1
    function=$2
    safe=1
    crash=2

    # Double n until the program crashes.
    while run_depth "$program" "$function" "$crash"
    do
        safe=$crash
        crash=$((crash * 2))
    done

    # Narrow the safe/crashing interval using its midpoint.
    while [ $((100 * (crash - safe))) -gt $((12 * safe)) ]
    do
        middle=$(((safe + crash) / 2))
        if run_depth "$program" "$function" "$middle"
        then
            safe=$middle
        else
            crash=$middle
        fi
    done

    percentage=$((100 * (crash - safe) / safe))
    printf "%-18s %-8s %-14d %-14d %d%%\n" \
        "$program" "$function" "$safe" "$crash" "$percentage"
}

echo "Maximum safe n summary"
printf "%-18s %-8s %-14s %-14s %s\n" \
    "Executable" "Function" "Largest safe" "First crash" "Gap"
find_max_n ./tests/depth-o0 0
find_max_n ./tests/depth-o0 1
find_max_n ./tests/depth-o2 0
find_max_n ./tests/depth-o2 1
