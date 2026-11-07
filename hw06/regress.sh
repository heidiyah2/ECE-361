#!/bin/sh
fail=0
for input in scenarios/*.txt; do
    name=$(basename "$input")
    if ./replay "$input" 2>&1 | diff - "expected/$name" > /dev/null; then
        echo "PASS  regression $name"
    else
        echo "FAIL  regression $name"
        fail=1
    fi
done
exit $fail
