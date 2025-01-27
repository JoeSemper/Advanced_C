#include <stdio.h>

int main(void)
{
    int n, c, a;

    scanf("%d", &n);

    int k = 26;
    int upper = 0;

    while ((c = getchar()) != '.')
    {
        if (c != ' ')
        {
            upper = (c >= 'A' && c <= 'Z');

            a = upper ? c - 'A' : c - 'a';

            a = (a + n) % k;

            c = upper ? a + 'A' : a + 'a';

        }

        printf("%c", c);
    }

    printf("%c", c);

    return 0;
}