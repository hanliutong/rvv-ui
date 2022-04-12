#include "../intrin_rvv_new.hpp"
#include <stdio.h>
float matA[] = {
    1, 2,  3,  4, 
    5, 6,  7,  8, 
    9, 10, 11, 12, 
    13,14, 15, 16};
float ans[8];
int main() {
    v_float32 v0 = v_load(matA);
    v_float32 v1 = v_load(matA+ VTraits<v_float32>::nlanes);
    v_float32 v2 = v_load(matA+ 2*VTraits<v_float32>::nlanes);
    v_float32 v3 = v_load(matA+ 3*VTraits<v_float32>::nlanes);
    // v_float32 vans = v_add(v0, v1, v2, v3);
    v_float32 vans = v_add(v0, v1, v2);
    // v_float32 vans = v_add(v0, v1);
    v_store(ans, vans);
    printf("%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n", ans[0],ans[1],ans[2],ans[3],ans[4],ans[5],ans[6],ans[7]);
    return 0;
}