#include "../inc/MathFunc.hpp"
#include "../inc/_Timer.hpp"
#include "../inc/SIMD.hpp"
#include "../inc/CPU.hpp"
#include <cstdlib>
#include <algorithm>
#include <cmath>



void MathFunc::min(Image* src, Image* dst, uint8_t val){
        StartTimer(MIN NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = std::min(src->red[i], val);
                dst->green[i] = std::min(src->green[i], val);
                dst->blue[i] = std::min(src->blue[i], val);
        }

        EndTimer  
        if(src->getChannels() == 4) Image::copyAlpha(src, dst); 
}

void MathFunc::minSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(MIN SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_min_epu8(v_val, v_red);
                v_green = _mm256_min_epu8(v_val, v_green);
                v_blue = _mm256_min_epu8(v_val, v_blue);
   
                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = std::min(src->red[i], val);
                dst->green[i] = std::min(src->green[i], val);
                dst->blue[i] = std::min(src->blue[i], val);
        }
        
        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void  MathFunc::max(Image* src, Image* dst, uint8_t val){
        StartTimer(MAX NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = std::max(src->red[i], val);
                dst->green[i] = std::max(src->green[i], val);
                dst->blue[i] = std::max(src->blue[i], val);
        }
        
        EndTimer 
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);        
}
void MathFunc::maxSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(MAX SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight())/ 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_max_epu8(v_val, v_red);
                v_green = _mm256_max_epu8(v_val, v_green);
                v_blue = _mm256_max_epu8(v_val, v_blue);
   
                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = std::max(src->red[i], val);
                dst->green[i] = std::max(src->green[i], val);
                dst->blue[i] = std::max(src->blue[i], val);
        }
        
        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void MathFunc::log(Image* src, Image* dst){
        StartTimer(LOG NO SIMD)

        uint8_t c  = 255 / std::log(255);

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = c * std::log(src->red[i] + 1);
                dst->green[i] = c * std::log(src->green[i] + 1);
                dst->blue[i] = c * std::log(src->blue[i] + 1);
        }

        EndTimer  
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}
void MathFunc::logSIMD(Image* src, Image* dst){
        StartTimer(LOG SIMD)
       
        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;
       
        _Float32 c  = 255 / std::log(255);

        __m256 v_c = _mm256_set1_ps(c);

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_log_epi8(v_red, v_c);
                v_green = _mm256_log_epi8(v_green, v_c);
                v_blue = _mm256_log_epi8(v_blue, v_c);

                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = c * std::log(src->red[i] + 1);
                dst->green[i] = c * std::log(src->green[i] + 1);
                dst->blue[i] = c * std::log(src->blue[i] + 1);      
        } 

        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void MathFunc::abs(Image* src, Image* dst){
        StartTimer(ABS NO SIMD)
        
        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = std::abs((int8_t)src->red[i]);
                dst->green[i] = std::abs((int8_t)src->green[i]);
                dst->blue[i] = std::abs((int8_t)src->blue[i]);
        }

        EndTimer 

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);        
}

void MathFunc::absSIMD(Image* src, Image* dst){
        StartTimer(ABS SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;
 
        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_abs_epi8(v_red);
                v_green = _mm256_abs_epi8(v_green);
                v_blue = _mm256_abs_epi8(v_blue);
   
                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = std::abs((int8_t)src->red[i]);
                dst->green[i] = std::abs((int8_t)src->green[i]);
                dst->blue[i] = std::abs((int8_t)src->blue[i]);
        }
        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}
/* does not work for large values because of float overflow, cant be bothered to fix this */
void MathFunc::pow(Image* src, Image* dst, _Float32 val){
        StartTimer(POW NO SIMD)
        
        double_t min = 0;
        double_t max = 255;

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                /* since downward conversion is undefined, make it behave like SIMD implementation */
                dst->red[i] = static_cast<uint8_t>(std::clamp(std::pow(src->red[i], val), min, max));
                dst->green[i] = static_cast<uint8_t>(std::clamp(std::pow(src->green[i], val), min, max));
                dst->blue[i] = static_cast<uint8_t>(std::clamp(std::pow(src->blue[i], val), min, max));   
        }

        EndTimer  

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void MathFunc::powSIMD(Image* src, Image* dst, _Float32 val){
        StartTimer(POW SIMD)
       
        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;
        double_t min = 0;
        double_t max = 255;
       
        __m256 v_val = _mm256_set1_ps(val);

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_pow_epi8(v_red, v_val);
                v_green = _mm256_pow_epi8(v_green, v_val);
                v_blue = _mm256_pow_epi8(v_blue, v_val);

                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        int16_t p_16;

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                p_16 = std::pow(src->red[i], val);
                dst->red[i] = static_cast<uint8_t>(std::clamp((double_t)p_16, min, max));
                p_16 = std::pow(src->green[i], val);
                dst->green[i] = static_cast<uint8_t>(std::clamp((double_t)p_16, min, max));
                p_16 = std::pow(src->blue[i], val);
                dst->blue[i] = static_cast<uint8_t>(std::clamp((double_t)p_16, min, max));      
        } 

        EndTimer  
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}