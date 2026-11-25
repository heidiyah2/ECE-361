#!/bin/sh
# Runs each test program named on the command line and shows its output.
# Every line starting with PASS or FAIL counts as one test. A program that
# exits non-zero without printing a FAIL (a crash, or a sanitizer error)
# counts as one failure. Ends with the one summary line the handout asks for,
# and exits non-zero if anything failed so make reports the failure too.
passed=0
failed=0
for t in "$@"; do
    out=$(./"$t" 2>&1)
    status=$?
    printf '%s\n' "$out"
    p=$(printf '%s\n' "$out" | grep -c '^PASS')
    f=$(printf '%s\n' "$out" | grep -c '^FAIL')
    if [ "$status" -ne 0 ] && [ "$f" -eq 0 ]; then
        echo "FAIL: $t exited with status $status"
        f=1
    fi
    passed=$((passed + p))
    failed=$((failed + f))
done
echo "make test: $passed passed, $failed failed"
[ "$failed" -eq 0 ]
