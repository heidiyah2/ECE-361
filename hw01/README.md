# HW 1 - ECE 361

## What This Library Does:
This library provides functions for manipulating 32-bit words and for decoding and displaying a thermostat's 16-bit status word. The tests in `tests/test_bits.c` check expected behavior and boundary cases for the functions.

## How to Build:

The Makefile uses `gcc` with `-std=c11 -Wall -Wextra`. From this directory, run `make` to compile the source files and build the `test_runner` executable.

## How to Run Tests:

Run `make test` to build and execute the test program. When finished, run `make clean` to remove the object files and `test_runner` executable.

For bit-field functions, valid widths are 1-32 and valid positions are 0-31. The sum of position and width must not exceed 32, so the selected bits stay within a 32-bit word. Boundary and out-of-range behavior for bit functions are described under File Contents. 

## File Contents:
### test.c
Various tests for behavior of functions organized by function, each tests boundary against expected. Produced PASS or FAIL and summarizes at end of each category and entire test.

### bits.c
Contains four functions for displaying, extracting, replacing, and interpreting bits in 32-bit words:
1. `print_binary` - takes a `uint32_t` value and a width, then prints the lowest `width` bits from most significant to least significant, grouping bits in fours with spaces. For example, `print_binary(0x2C, 8)` prints `0010 1100`.
    - Boundary behavior: Widths from 1 through 32 are accepted. For a width below 1 or above 32, it prints an invalid-range message and returns without printing the bit pattern.
2. `get_field` - takes a word, a starting bit position, and a width. It extracts bits from `pos` through `pos + width - 1`, then shifts the result down so the field starts at bit 0.
    - Boundary behavior: Valid positions are 0-31 and valid widths are 1-32, with `pos + width` no greater than 32. Invalid arguments return `UINT32_MAX` as an error value.
3. `set_field` - takes a word, a starting bit position, a width, and a value. It replaces the selected field with the lowest `width` bits of `value` and leaves the other word bits unchanged.
    - Boundary behavior: Valid positions are 0-31 and valid widths are 1-32, with `pos + width` no greater than 32. Invalid arguments return `UINT32_MAX` as an error value.
4. `sign_extend` - takes a value and a width, interprets the lowest `width` bits as a two's-complement number, and returns the signed result as an `int32_t`. For example, `sign_extend(0xF8, 8)` returns `-8`.
    - Boundary behavior: Widths from 1 through 32 are accepted. A width of 0 or greater than 32 returns `-1`. At width 32, all 32 bits of the input are used.

Notes on boundary behavior decisions:
- `UINT32_MAX` was chosen as the out-of-range return value for `get_field` and `set_field` to 1) avoid setting bits to 0 and giving the user a sense of "all's clear!" when that might not be the case, and 2) to hopefully make issues more noticeable and assist debugging.
- `-1` was chosen as the out-of-range return value for `sign_extend` as it's a common value related to errors, and the return type allowed it. This could cause issues if the result expected for this function is `-1`, but hopefully the likelihood is low as this function is used to decode the temperature status field. 

### status.c
Contains two functions for decoding and displaying the thermostat's 16-bit status word:
1. `status_unpack` - takes a `uint16_t` word and returns a `status_t` containing the decoded fields. It uses `get_field` to extract HEAT (bit 0), COOL (bit 1), FAN (bit 2), FAULT (bit 3), MODE (bits 4-6), the reserved bit (bit 7), and SETPOINT (bits 8-15). The setpoint is interpreted as an 8-bit two's-complement temperature in degrees Celsius.
    - MODE values 0-4 correspond to OFF, HEAT, COOL, AUTO, and FAN_ONLY. Values 5-7 are invalid and are reported as `STATUS_MODE_INVALID` (`-1`).
    - If the reserved bit is 1, `STATUS_RESERVED` is true; the bit is preserved in the result rather than treated as a mode error.
2. `status_print` - takes a `status_t` and prints each field in struct order. It displays the four on/off flags, the numeric mode and its name, whether the reserved bit is clear or set, and the signed setpoint with a degrees Celsius symbol. A set reserved bit is printed as "set" to make it visible to the user.