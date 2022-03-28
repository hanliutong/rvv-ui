#ifdef _FIXED
#include "intrin_rvv.hpp"
#else
#include "intrin_rvv_new.hpp"
#endif

#include <stdio.h>

template <typename T>
T matX[] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,
            17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
template <typename T>
T matY[] =  {17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16};
template <typename T>
T output[32] = {0};
template <typename T>
T output_golden[32] = {0};

// y = a * x + y
template <class v_type>
void axpy(  size_t n, typename VTraits<v_type>::lane_type a,  
            const typename VTraits<v_type>::lane_type* x, 
            const typename VTraits<v_type>::lane_type* y, 
            typename VTraits<v_type>::lane_type* output)
{

    size_t  vl = VTraits<v_type>::nlanes;
    v_type vx, vy;
    int loopCnt = 0;

    for (; n > 0; n -=  vl, x +=  vl, y +=  vl, output += vl) {
        loopCnt++;
        vx = v_load(x);
        vy = v_load(y);
        vy = v_fma(a, vx, vy);
        v_store(output, vy);
    }
    printf("nlanes=%u;\tloop %d time(s)\n", VTraits<v_type>::nlanes, loopCnt);
}

template <class T>
void axpy_golden(size_t n, const T a, const T *x, const T *y, T *output) { 
    for (size_t i = 0; i < n; ++i)
        output[i] = a * x[i] + y[i];
}

int main() {
    axpy_golden<float>(32, 1, matX<float>, matY<float>, output_golden<float>);
    axpy<v_float32>(32, 1, matX<float>, matY<float>, output<float>);
    bool pass = true;
    for(int i = 0; i < 32; ++i) {
        pass &= output<float>[i] == output_golden<float>[i];
    }
    printf("F32: %s\n", pass ? "PASS" : "FAIL");

    axpy_golden<short>(32, 1, matX<short>, matY<short>, output_golden<short>);
    axpy<v_int16>(32, 1, matX<short>, matY<short>, output<short>);
    pass = true;
    for(int i = 0; i < 32; ++i) {
        pass &= output<short>[i] == output_golden<short>[i];
    }
    printf("I16: %s\n", pass ? "PASS" : "FAIL");

    return 0;
}