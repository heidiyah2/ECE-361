# HW 1 for ECE 361

// add what library does, how to build, how to run tests, valid ranges of inputs, behavior at boundaries, and out-of-range behavior. And how status_unpack reports and invalid mode.


### test.c
Various tests for behavior of functions organized by function, each tests boundary against expected. Produced PASS or FAIL and summarizes at end of each category and entire test.

### bits.c
Contains 4 functions:
1. print_binary - takes arguments uint32 x and int width and prints the lowest width bits of x, most significant bit first, in groups of four separated by a space. Ex: print_binary(0x2C, 8) gives 0010 1100.
2. get_field - takes arguments uint32 word, int pos, and int width to return bits pos to pos+width-1 of word, shifted down to bit 0.
    - Boundary behavior: For invalid arguments (pos > 31, width = 0, width > 32, pos + width > 32) field = 0xFFFFFFFF. This is so if something goes awry a false "all clear" (bits = 0) is not given and hopefully debugging is made easier.
3. set_field - takes arguments uint32 word, int pos, int width, uint32 value and returns word with bits pos to pos+width -1 replaced by the lowest width bits of value. All other bits are unchanged.
    - Boundary behavior: For invalid arguments (pos > 31, width = 0, width > 32, pos + width > 32) word = 0xFFFFFFFF, for the same reason as get_field.
4. sign_extend - takes arguments uint32 value, int width and interprets the lowest width bits of value as a two's complement number and returns it as an int32. Ex: sign_extend(0xF8, 8) returns -8.
    - Boundary behavior: For invalid widths (0 or >32) return -1 to signify error. For width = 32 original value is returned (mask set to max).

### status.c
Contains the function status_unpack(uint16_t, word) which returns the struct status_t and utilizes functions from bits.c to unpack the 16-bit word a thermostat reports. Invalid mode values give the value -1. Reserved bit of 1 shows "set" in print_status to alert user this bit is in use, presumably they should know what to do with that.