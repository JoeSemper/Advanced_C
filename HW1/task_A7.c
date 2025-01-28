#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t n, k, result;

    scanf("%d", &n);
    scanf("%d", &result);

    for(int i = 1; i < n; i++) 
    {
        scanf("%d", &k);

        result ^= k;
    }

    printf("%u\n", result);

    return 0;
}