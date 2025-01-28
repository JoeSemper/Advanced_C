#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t n, result;
    uint32_t mask = 0xff000000;
 
    scanf("%d", &n);

    result = n ^ mask;

    printf("%u\n", result);

    return 0;
}