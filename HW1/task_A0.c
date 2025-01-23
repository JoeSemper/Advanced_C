#include <stdio.h>

int main(void)
{
    int n, a, max;
    int counter = 1;

    scanf("%d", &n);

    scanf("%d", &max);

    for (int i = 1; i < n; i++)
    {
        scanf("%d", &a);

        if (a > max)
        {
            max = a;
            counter = 1;
        }
        else if (a == max)
        {
            counter++;
        }
    }

    printf("%d", counter);

    return 0;
}