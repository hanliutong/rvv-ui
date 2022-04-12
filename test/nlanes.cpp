////////////// Reverse //////////////
#include "../intrin_rvv_new.hpp"
#include <stdio.h>
inline v_float32 v_reverse(const v_float32& a)  \
{ \
    float buf[VTraits<v_float32>::max_nlanes]; \
    float temp; \
    v_store(buf, a);
    for (size_t i = 0; i <= VTraits<v_float32>::nlanes / 2; i++)
    {
        temp = buf[VTraits<v_float32>::nlanes-i-1];
        buf[VTraits<v_float32>::nlanes-i-1] = buf[i];
        buf[i] = temp;
    }
    return v_load(buf); \
}

float matA[] = {
    1, 2,  3,  4, 
    5, 6,  7,  8, 
    9, 10, 11, 12, 
    13,14, 15, 16};
float ans[8];
int main() {
    // v_\w+x\d+\s*\(\s*\d+
    v_float32 vans = v_load(matA);
    vans = v_reverse(vans);
    v_store(ans, vans);
    printf("%u\n", VTraits<v_float32>::max_nlanes);
    printf("%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n", ans[0],ans[1],ans[2],ans[3],ans[4],ans[5],ans[6],ans[7]);
    return 0;
}