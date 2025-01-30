#include <stdio.h>
#include <stdint.h>

int extractExp(float f) {

    uint32_t mask = 0x000000ff;

    union u_float {
        float f;
        uint32_t u;
    };

    union u_float fl;

    fl.f = f;

    return ((fl.u >> 23) & mask);
}

int main(void)
{
    float f;

    scanf("%f", &f);
    
    printf("%d\n", extractExp(f));

    return 0;
}