#include "bits.h"

void print_binary(uint32_t x, int width)
{
    uint32_t mask;
    // check for out of bounds width
    if (width < 1 || width > 32) { 
        printf("Cannot print binary; width is not between 1 and 32.\n");
        return;
    }

    mask = UINT32_C(1) << (width - 1u); // specify min 32 bits for shift (guaranteed for 1u is >=16 bits only, implementation-defined)

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

    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32)
        field = UINT32_MAX;
    else {
        mask = width == 32 ? UINT32_MAX : (UINT32_C(1) << width) - 1u;
        field = (word >> pos) & mask;
    }

    return field;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    uint32_t mask, clear, set;

    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32)
        word = 0xFFFFFFFF;
    else {
        mask = width == 32 ? UINT32_MAX : (UINT32_C(1) << width) - 1u;
        clear = word & ~(mask << pos);
        set = (value &= mask ) << pos;
        word = clear | set;
    }

    return word;
}

int32_t sign_extend(uint32_t value, int width)
{
    uint32_t mask, sign_bit;

    // out of bounds handling
    if (width < 1 || width > 32)
        return -1;

    mask = width == 32 ? UINT32_MAX : (1u << width) - 1u; // condition ? value_if_true : value_if_false
    value &= mask;
    sign_bit = 1u << (width - 1);

    if ((value & sign_bit) != 0)
        value |= ~mask;

    return (int32_t)value;
}