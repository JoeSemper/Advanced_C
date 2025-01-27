#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t n, k, max;
    uint32_t mask = 0xffffffff;

    scanf("%d%d", &n, &k);

    mask = mask >> 32 - k;

    max = mask & n;

    for(int i = 1; i <= 32-k; i++)
    {
        n = n >> 1;
        max = (mask & n) > max ? mask & n : max;
    }

    printf("%u\n", max);

    return 0;
}