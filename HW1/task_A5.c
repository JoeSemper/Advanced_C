#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t n;
    int result = 0;

    scanf("%d", &n);

    do
    {
        result = result + n % 2;
        n /= 2;
    } while (n > 0);

    printf("%d\n", result);

    return 0;
}