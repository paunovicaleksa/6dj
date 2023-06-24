#include "../inc/Arithmetic.hpp"
#include "../inc/_Timer.hpp"
#include "../inc/SIMD.hpp"
#include "../inc/CPU.hpp"
#include <cstdlib>
#include <algorithm>



void Arithmetic::add(Image* src, Image* dst, uint8_t val){

        StartTimer(ADD NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] + val;
                dst->green[i] = src->green[i] + val;
                dst->blue[i] = src->blue[i] + val;
        }
        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);

}

void Arithmetic::addSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(ADD SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_add_epi8(v_red, v_val);
                v_green = _mm256_add_epi8(v_green, v_val);
                v_blue = _mm256_add_epi8(v_blue, v_val);

                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] + val;
                dst->green[i] = src->green[i] + val;
                dst->blue[i] = src->blue[i] + val;
        }
 
        EndTimer 
        
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::sub(Image* src, Image* dst, uint8_t val){
        StartTimer(SUB NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] - val;
                dst->green[i] = src->green[i] - val;
                dst->blue[i] = src->blue[i] - val;
        }

        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::subSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(SUB SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_sub_epi8(v_red, v_val);
                v_green = _mm256_sub_epi8(v_green, v_val);
                v_blue = _mm256_sub_epi8(v_blue, v_val);

                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] - val;
                dst->green[i] = src->green[i] - val;
                dst->blue[i] = src->blue[i] - val;
        }

        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::invSub(Image* src, Image* dst, uint8_t val){
        StartTimer(INV SUB NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = val - src->red[i];
                dst->green[i] = val - src->green[i];
                dst->blue[i] = val - src->blue[i];
        }

        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::invSubSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(INV SUB SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_sub_epi8(v_val, v_red);
                v_green = _mm256_sub_epi8(v_val, v_green);
                v_blue = _mm256_sub_epi8(v_val, v_blue);

                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = val - src->red[i];
                dst->green[i] = val - src->green[i];
                dst->blue[i] = val - src->blue[i];
        }
        
        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::mul(Image* src, Image* dst, uint8_t val){
        StartTimer(MUL NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] * val;
                dst->green[i] = src->green[i] * val;
                dst->blue[i] = src->blue[i] * val;
        }

        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::mulSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(MUL SIMD)

        int32_t loop_size = (src->getWidth() * src->getHeight() / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_mullo_epi8( v_val, v_red);
                v_green = _mm256_mullo_epi8(v_val, v_green);
                v_blue = _mm256_mullo_epi8(v_val, v_blue);

                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] * val;
                dst->green[i] = src->green[i] * val;
                dst->blue[i] = src->blue[i] * val;
        }

        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::div(Image* src, Image* dst, uint8_t val){
        StartTimer(DIV NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] / val;
                dst->green[i] = src->green[i] / val;
                dst->blue[i] = src->blue[i] / val;
        }

        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::divSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(DIV SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_div_epi8(v_red, v_val);
                v_green = _mm256_div_epi8(v_green, v_val);
                v_blue = _mm256_div_epi8(v_blue, v_val);

                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] / val;
                dst->green[i] = src->green[i] / val;
                dst->blue[i] = src->blue[i] / val;
        }
        

        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::invDiv(Image* src, Image* dst, uint8_t val){
        StartTimer(INV DIV NO SIMD)

        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] > 0? (val / src->red[i]) : val;
                dst->green[i] = src->green[i] > 0? (val / src->green[i]) : val;
                dst->blue[i] = src->blue[i] > 0? (val / src->blue[i]) : val;
        }

        EndTimer   

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Arithmetic::invDivSIMD(Image* src, Image* dst, uint8_t val){
        StartTimer(INV DIV SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
               
                v_red = _mm256_div_epi8(v_val, v_red);
                v_green = _mm256_div_epi8(v_val, v_green);
                v_blue = _mm256_div_epi8(v_val, v_blue);
   
                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_red);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_green);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_blue);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = src->red[i] > 0? (val / src->red[i]) : val;
                dst->green[i] = src->green[i] > 0? (val / src->green[i]) : val;
                dst->blue[i] = src->blue[i] > 0? (val / src->blue[i]) : val;
        }
        
        EndTimer

        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}