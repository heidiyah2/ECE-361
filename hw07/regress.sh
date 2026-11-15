#!/bin/sh
# Runs ./balance on every file in inputs/ and compares its output and exit
# status with the file of the same name in expected/. Each expected file is
# the one line balance prints, then "exit status N".
fail=0
for input in inputs/*.txt; do
    name=$(basename "$input")
    actual=$(./balance < "$input" 2>&1; echo "exit status $?")
    if printf '%s\n' "$actual" | diff - "expected/$name" > /dev/null; then
        echo "PASS  regression $name"
    else
        echo "FAIL  regression $name"
        printf '%s\n' "$actual" | diff - "expected/$name" | sed 's/^/      /'
        fail=1
    fi
done
exit $fail
