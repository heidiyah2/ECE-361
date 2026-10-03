# HW 1 for ECE 361

### test.c
Various tests for behavior of functions.

### bits.c
Contains 4 functions:
1. print_binary - takes arguments uint32 x and int width and prints the lowest width bits of x, most significant bit first, in groups of four separated by a space. Ex: print_binary(0x2C, 8) gives 0010 1100.
2. get_field - takes arguments uint32 word, int pos, and int width to return bits pos to pos+width-1 of word, shifted down to bit 0.
    - Mask RHS of word by desired width
    - Shift bits of word to right by pos so desired section of word and mask align
    - Compare to isolate bits in width
    - Boundary behavior: for invalid arguments (pos > 31, width = 0, width > 32, pos + width > 32) field = 0xFFFFFFFF. Status_unpack will contain check for invalid arguments.
3. set_filed - takes arguments uint32 word, int pos, int width, uint32 value and returns word with bits pos to pos+width -1 replaced by the lowest width bits of value. All other bits are unchanged.
4. sign_extend - takes arguments uint32 value, int width and interprets the lowest width bits of value as a two's complement number and returns it as an int32. Ex: sign_extend(0xF8, 8) returns -8.