#include <stdio.h>
#include <string.h>

#define SIZE 1000

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void zFunction(char *s, int z[])
{
    int n = strlen(s);
    for (int i = 1, l = 0, r = 0; i < n; ++i)
    {
        if (i <= r)
            z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
}

void printZ(char *s, int z[])
{
int n =  strlen(s);
    for (int i=0; i<n; ++i)
        printf("  %c,",s[i]);
    printf("\n");
    for (int i=0; i<n; ++i)
        printf("%3d," ,z[i]);
    printf("\n");        
}

int maxPrefix(char *t, char *p)
{
    char s[SIZE + SIZE + 1] = {0};
    int z[SIZE + SIZE + 1] = {0};

    int tlen = strlen(t);
    int plen = strlen(p);

    sprintf(s, "%s#%s", t, p);

    zFunction(s, z);
    // printZ(s, z);

    int size = strlen(s);
    int i = min(tlen, plen);

    for (i; i < size; i++)
    {
        if(z[i] > 0 && z[i] == size - i) {
            return z[i];
        }
    }

    return 0;
}

int main(void)
{
    char t[SIZE] = {0}, p[SIZE] = {0};

    scanf("%s", &t);
    scanf("%s", &p);

    printf("%d %d", maxPrefix(t, p), maxPrefix(p, t));

    return 0;
}