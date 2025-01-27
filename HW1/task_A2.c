#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t n, k;

    scanf("%d%d", &n, &k);

    n = (n >> k)|(n << (32 - k));

    printf("%u", n);

    return 0;
}