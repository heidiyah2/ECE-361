#include "bits.h"
#include <inttypes.h>

int main(void)
{
    /*
    print_binary(0x2C, 8); // test against given example
    print_binary(0x2C, -1); //test lower width bound
    print_binary(0x2C, 33); //test upper width bound
    print_binary(0x2C, 10); //test when width % 4 != 0
    */

    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 4, 8)); // answer should be 0x000000BC
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 1, 33)); // test width > 32, should be 0xFFFFFFFF
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 32, 4)); // test pos > 31, should be 0xFFFFFFFF
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 30, 4)); // test pos + width > 32, should be 0xFFFFFFFF
    printf("0x%08" PRIX32 "\n", get_field(0xABCD, 4, 0)); // test width = 0, should be 0xFFFFFFFF

    return 0;
}