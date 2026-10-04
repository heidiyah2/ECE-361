I used the Chat in VS Code for code generation and questions on improvements and interpretations, and Gemini for general reminders of various C syntax (like how to print a single character) and git commands.


### test_bits.c
- Used to figure out how to print uint32_t as hex value.
- Used to move to folder tests/ and rename to test_bits.c (originally test.c before reading part 4 of the instructions).
- Generated tests using assert to 

### bits.h
- Used VS Code Chat to populate bits.h with desired protection, libraries, and prototypes.

### bits.c
- Had AI check my design of print_binary and sign_extend for functionality.
- It tried to change sign_extend to use int64_t as intermediate step to allow calculation of negative number by subtracting -2^n, had to correct it to check my method using a mask and bitwise manipulation instead.

### status.h
- Used to check my syntax for struct and function prototypes.
- Used to rename members in struct to all caps with STATUS_ preceding names after discovering off and mode were already in use by C.

### status.c
- Used to generate enum constants for field widths and positions quickly.
- Generated repetitive calls of get_field with appropriate constants for desired fields to avoid lengthy typing.
- Generated status_mode_name to be able to print results of status_unpack.


### Makefile
- Generated file with desired gcc warning tags and dependencies for compilation and linking.

