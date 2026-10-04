#include "bits.h"
#include <inttypes.h>

int main(void)
{
    /*
    print_binary(0x2C, 8); // general use, answer should be 0011 1100
    print_binary(0x2C, -1); //test lower width bound
    print_binary(0x2C, 33); //test upper width bound
    print_binary(0x2C, 10); //test when width % 4 != 0
    */

    /*
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 4, 8)); // general use, answer should be 0x000000BC
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 1, 33)); // test width > 32, should be 0xFFFFFFFF for all below
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 32, 4)); // test pos > 31
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 30, 4)); // test pos + width > 32
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 4, 0)); // test width = 0
    */

    /*
    printf("0x%08" PRIX32 "\n", set_field(0x12345678, 8, 8, 0x1FF)); // general use, should give 0x1234FF78
    printf("0x%08" PRIX32 "\n", set_field(0x12345678, 8, 33, 0x1FF)); // test width > 32, should return 0xFFFFFFFF for all below
    printf("0x%08" PRIX32 "\n", set_field(0x12345678, 32, 1, 0x1FF)); // test pos > 31
    printf("0x%08" PRIX32 "\n", set_field(0x12345678, 12, 24, 0x1FF)); // pos + width > 32
    printf("0x%08" PRIX32 "\n", set_field(0x12345678, 8, 0, 0x1FF)); // test width = 0
    */

    return 0;
}