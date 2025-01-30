#include <stdio.h>
#include <stdint.h>
#include <math.h>

struct pack_array
{
    uint32_t array;
    uint32_t count0 : 8;
    uint32_t count1 : 8;
};

void array2struct(int arr[], struct pack_array *pack)
{
    uint32_t count0 = 0;
    uint32_t count1 = 0;
    uint32_t array = 0;

    for (int i = 0; i < 32; i++)
    {
        array = array + arr[i] * pow(2, i);
        if (arr[i] == 0)
        {
            count0++;
        }
        else
        {
            count1++;
        }
    }

    (*pack).array = array;
    (*pack).count0 = count0;
    (*pack).count1 = count1;
}

int main(void)
{
    int n;
    int arr[32];
    struct pack_array pack;

    for (int i = 31; i >= 0; i--)
    {
        scanf("%d", &arr[i]);
    }

    array2struct(arr, &pack);

    printf("%u %u %u", pack.array, pack.count0, pack.count1);

    return 0;
}