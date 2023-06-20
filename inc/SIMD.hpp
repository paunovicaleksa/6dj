
#include <emmintrin.h>
#include <immintrin.h>
#include "avx_mathfun.h"

inline  __attribute__((always_inline)) 
__m256i _mm256_mullo_epi8(__m256i a, __m256i b){
    // unpack and multiply
    __m256i dst_even = _mm256_mullo_epi16(a, b);
    __m256i dst_odd = _mm256_mullo_epi16(_mm256_srli_epi16(a, 8),_mm256_srli_epi16(b, 8));
    
    // repack
     return _mm256_or_si256(_mm256_slli_epi16(dst_odd, 8), _mm256_and_si256(dst_even, _mm256_set1_epi16(0xff)));
}

//extract 32 8bit values into 8 32bit values?
inline  __attribute__((always_inline)) 
void _mm256_splitpu8_epi32(__m256i* dst, __m256i src8){
      
        __m128i src8_lower = _mm256_extracti128_si256(src8, 0);
        __m128i src8_higher = _mm256_extracti128_si256(src8, 1);

        dst[0] = _mm256_cvtepu8_epi32(src8_lower);
        dst[1] = _mm256_cvtepu8_epi32(_mm_unpackhi_epi64(src8_lower, src8_lower)); 

        dst[2] = _mm256_cvtepu8_epi32(src8_higher);
        dst[3] = _mm256_cvtepu8_epi32(_mm_unpackhi_epi64(src8_higher, src8_higher));      
}

inline  __attribute__((always_inline)) 
__m256i _mm256_cvtepi32_epu8(__m256i* src){
        __m256i packed_low = _mm256_packus_epi32(src[0], src[1]);
        __m256i packed_low_permuted = _mm256_permute4x64_epi64(packed_low, 0xd8);

        __m256i packed_high = _mm256_packus_epi32(src[2], src[3]);
        __m256i packed_high_permuted = _mm256_permute4x64_epi64(packed_high, 0xd8);

        __m256i packed = _mm256_packus_epi16(packed_low_permuted, packed_high_permuted);
        __m256i result = _mm256_permute4x64_epi64(packed, 0xd8);

        return result;
}


inline  __attribute__((always_inline)) 
__m256i _mm256_div_epi8(__m256i a, __m256i b){

        __m256i dst_a[4];
        __m256i dst_b[4];

        _mm256_splitpu8_epi32(dst_a, a);
        _mm256_splitpu8_epi32(dst_b, b);

        __m256 f_b[4];
        __m256 f_a[4];

        __m256i i_res[4];

        __m256i v_one = _mm256_set1_epi32(1);

        for(int32_t i = 0; i < 4; i++){

                dst_b[i] = _mm256_max_epi32(dst_b[i], v_one);

                f_b[i] = _mm256_cvtepi32_ps(dst_b[i]);
                f_a[i] = _mm256_cvtepi32_ps(dst_a[i]);
        
                i_res[i] = _mm256_cvtps_epi32(_mm256_div_ps(f_a[i], f_b[i]));
        }    
        
        return _mm256_cvtepi32_epu8(i_res);
}

inline  __attribute__((always_inline)) 
__m256i _mm256_log_epi8(__m256i a, __m256i c){
        __m256i dst_a[4];
        __m256i dst_c[4];

        _mm256_splitpu8_epi32(dst_a, a);
        _mm256_splitpu8_epi32(dst_c, c);

        __m256i log_a[4];

        for(int32_t i = 0; i < 4; i++){
                /* TODO: call log for a, multiply with constant, convert back to m256i and return */
        }

}