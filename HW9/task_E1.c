#include <stdio.h>

int main(void)
{
    int n, k;
    int result = 0;

    scanf("%d", &n);
    scanf("%d", &k);

    for (int i = 1; i <= n; i++)
    {
        int zeros = 0;
        int a = i;

        while (a > 0)
        {
            if (a % 2 == 0)
                zeros++;

            a /= 2;
        }

        if (zeros == k)
            result++;
    }

    printf("%d", result);

    return 0;
}