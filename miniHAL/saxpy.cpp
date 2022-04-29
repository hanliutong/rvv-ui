#include <stdio.h>
#include <assert.h>
#include "intrin.hpp"
using namespace cv;

float matX[] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,
            17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};

float matY[] =  {17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16};

float output[32] = {0};

int main() {
    int a = 1;
    const size_t n = 32;
    size_t i = 0;
    int loopCnt = 0;

    #if CV_SIMD || CV_SIMD_SCALABLE
        SCALABLE_HAL_BEGIN
        printf("SIMD is used\n");
        size_t vl = VTraits<v_float32>::nlanes;
        v_float32 vx, vy;
        v_float32 va = v_setall_f32(a);
        float* outputPtr = output;
        for (; i < n; i +=  vl, outputPtr += vl) {
            loopCnt++;
            vx = v_load(matX+i);
            vy = v_load(matY+i);
            vy = v_add(vy, v_mul(va, vx));
            // vy = v_fma(va, vx, vy); //is also ok.
            v_store(outputPtr, vy);
        }
        SCALABLE_HAL_END

        #if CV_SIMD_SCALABLE
            printf("scalable hal is used\n");
        #endif

    #endif

    for (; i < n; ++i) {
        output[i] = a * matX[i] + matY[i];
        loopCnt++;
    }
        
    
    // check
    for (size_t i = 0; i < n; ++i)
        assert(output[i] == a * matX[i] + matY[i]);
    printf("passed, loopCNT=%d\n\n", loopCnt);
    return 0;
}