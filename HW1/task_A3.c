#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t n, k;
    uint32_t mask = 0xffffffff;

    scanf("%d%d", &n, &k);

    mask = mask >> 32 - k;

    n = mask & n;

    printf("%u\n", n);

    return 0;
}