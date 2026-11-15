#include <stdio.h>

#include "istack.h"

int main(void)
{
    // create empty stack and variables
    istack_t *s = istack_create();
    if (s == NULL) {
        fprintf(stdout, "error: could not create stack\n");
        return 1;
    }
    int c;              // getchar returns int
    int line = 1;       // current line
    int col = 0;        // current column
    int pairs = 0;      // matched pairs so far
    size_t deepest = 0; // most brackets open at once
    int status = 0;     // return status for main
    int result = 0;     // return status of istack calls
    int open = 0;       // holds open or leftover bracket
    int expected = 0;   // the right bracket that matches open
    size_t count;       // current count of stack

    // read characters
    while ((c = getchar()) != EOF) {
        col++;
        switch (c) {
            case '(': case '{': case '[':   // push left brackets
                result = istack_push(s, c);
                if (result != 0) {
                    fprintf(stdout, "error: push failed\n");
                    status = 1;
                    break;
                }
                // update deepest to match count
                count = istack_count(s);
                if (count > deepest) {
                    deepest = count;
                }
                break;
            case ')': case '}': case ']':   // pop last left bracket
                result = istack_pop(s, &open);
                if (result != 0) {          // if empty
                    fprintf(stdout, "line %d, column %d: found '%c', nothing open\n", line, col, c);
                    status = 1;
                    break;
                }
                // find the right bracket that matches open
                switch (open) {
                    case '(':
                        expected = ')';
                        break;
                    case '{':
                        expected = '}';
                        break;
                    case '[':
                        expected = ']';
                        break;
                }
                // compare it with the right bracket found
                if (c != expected) {
                    fprintf(stdout, "line %d, column %d: found '%c', expected '%c'\n", line, col, c, expected);
                    status = 1;
                    break;
                }
                pairs++;    // update pair #
                break;
            case '\n':
                line++;
                col = 0;
                break;
            default:
                break;
        }
        if (status != 0) {
            break;
        }
    }
    // print for stack not empty or empty, only if no error stopped the loop
    if (status == 0) {
        count = istack_count(s);
        if (count != (size_t)0) {
            istack_peek(s, &open);  // can't fail due to count check
            fprintf(stdout, "end of input: %zu still open, innermost '%c'\n", count, open);
            status = 1;
        } else {
            fprintf(stdout, "balanced: %d pairs, deepest %zu\n", pairs, deepest);
        }
    }
    istack_destroy(s);
    return status;
}
