#include <cassert>

#include "uint256_t.h"

int main(){
    {
        uint256_t a{ uint128_t{ 0, 0 }, uint128_t{  3, 16416659884907707943ULL } };
        uint256_t b{ uint128_t{ 0, 0 }, uint128_t{ 46,  9408422516637784225ULL } };
        uint256_t r = a * b;

        uint256_t correct_result           { uint128_t{ 0, 180 }, uint128_t{ 17001386576136348907ULL, 6321838829247757959ULL } };
        uint256_t what_is_actually_returned{ uint128_t{ 0, 179 }, uint128_t{ 17001386576136348907ULL, 6321838829247757959ULL } };
        assert(r == correct_result);
    }

    return 0;
}