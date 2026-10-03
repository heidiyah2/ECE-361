/*  File: bits.c 
    Purpose: A small bit-manipulation library
    Author: Heidi Schlunt*/

    #include "bits.h"

void print_binary(uint32_t x, int width)
{
    uint32_t mask;
    // check for out of bounds width
    if (width < 1 || width > 32) { 
        printf("Invalid range for width\n");
        return;
    }

    mask = 1u << (width - 1u);

    for (int bits_left = width; bits_left > 0; --bits_left) {
        if (bits_left < width && bits_left % 4 == 0) {
            putchar(' ');
        }

        if ((x & mask) != 0) 
            putchar('1');
        else 
            putchar('0');

        mask >>= 1; // moves mask bit to right to match position of bits_left
    }

    printf("\n");

    return;
}

uint32_t get_field(uint32_t word, int pos, int width) 
{
    uint32_t field, mask;

    if (width > 32 || pos > 31 || pos + width > 32 || width == 0)
        field = 0xFFFFFFFF;
    else {
        mask = (1u << width) - 1u;
        field = (word >> pos) & mask;
    }

    return field;
}