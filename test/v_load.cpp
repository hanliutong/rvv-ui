#include "../intrin_rvv_new.hpp"
#include <stdio.h>
float matA[] = {
    1, 2,  3,  4, 
    5, 6,  7,  8, 
    9, 10, 11, 12, 
    13,14, 15, 16};
float ans[8];
int main() {
    v_float32 vans = v_load(matA[0], matA[1], matA[2], matA[3]);
    v_store(ans, vans);
    printf("%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n", ans[0],ans[1],ans[2],ans[3],ans[4],ans[5],ans[6],ans[7]);
    return 0;
}

// regular expr. for constructor like vfloat32x4(1,2,3,4): 
// v_\w+x\d+\s*\(\s*\d+