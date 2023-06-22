#include "../inc/Filters.hpp"
#include "../inc/_Timer.hpp"
#include "../inc/SIMD.hpp"
#include "../inc/CPU.hpp"
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <thread>

void Filters::invert(Image* src, Image* dst){
        StartTimer(INVERT NO SIMD)

        uint8_t max = -1;
        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->red[i] = max - src->red[i];
                dst->green[i] = max - src->green[i];
                dst->blue[i] = max - src->blue[i];
        }

        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Filters::invertSIMD(Image* src, Image* dst){
        StartTimer(INVERT SIMD)

        int32_t loop_size = ((src->getWidth() * src->getHeight())/ 32 ) * 32;
        uint8_t max = -1;

        __m256i v_val =  _mm256_set1_epi8(max); //sets 32 members of vval to val

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
                dst->red[i] = max - src->red[i];
                dst->green[i] = max - src->green[i];
                dst->blue[i] = max - src->blue[i];
        } 

        EndTimer    
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Filters::grayscale(Image* src, Image* dst){
        StartTimer(GRAYSCALE NO SIMD)

        _Float32 g_constant = 1./3;
        for(int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                _Float32 R = src->red[i] * g_constant;
                _Float32 G = src->green[i] * g_constant;
                _Float32 B = src->blue[i] * g_constant;

                uint8_t I = (R + G + B);

                dst->red[i] = dst->green[i] = dst->blue[i] = I;
        }        
        
        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Filters::grayscaleSIMD(Image* src, Image* dst){
        StartTimer(GRAYSCALE SIMD)
        _Float32 g_constant = 1./3;

        int32_t loop_size = ((src->getWidth() * src->getHeight()) / 32 ) * 32;
        __m256 v_gray = _mm256_set1_ps(g_constant);
        for(int32_t i = 0; i < loop_size; i += 32){
                __m256i v_red = _mm256_loadu_si256((__m256i*)&(src->red[i]));

                __m256i v_green = _mm256_loadu_si256((__m256i*)&(src->green[i]));

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&(src->blue[i]));
                
                __m256i dst_r[4];
                __m256i dst_g[4];
                __m256i dst_b[4];

                _mm256_splitpu8_epi32(dst_r, v_red);
                _mm256_splitpu8_epi32(dst_g, v_green);
                _mm256_splitpu8_epi32(dst_b, v_blue);

                __m256 f_r[4];
                __m256 f_g[4];
                __m256 f_b[4];

                __m256i sum_i[4];

                for(int32_t i = 0; i < 4; i++){
                        f_r[i] = _mm256_cvtepi32_ps(dst_r[i]);
                        f_g[i] = _mm256_cvtepi32_ps(dst_g[i]);
                        f_b[i] = _mm256_cvtepi32_ps(dst_b[i]);

                        __m256 sum = _mm256_mul_ps(v_gray, f_r[i]);
                        sum = _mm256_fmadd_ps(v_gray, f_g[i], sum);
                        sum = _mm256_fmadd_ps(v_gray, f_b[i], sum);

                        sum_i[i] = _mm256_cvtps_epi32(sum);
                }

                __m256i v_i = _mm256_cvtepi32_epu8(sum_i);
                _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_i);
                _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_i);
                _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_i);
        }

        for(int32_t i = loop_size; i < (src->getWidth() * src->getHeight()); i++){
                _Float32 R = src->red[i] * g_constant;
                _Float32 G = src->green[i] * g_constant;
                _Float32 B = src->blue[i] * g_constant;

                uint8_t I = (R + G + B);

                dst->red[i] = dst->green[i] = dst->blue[i] = I;
        } 

        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Filters::applyKernel(Image* src, Image* dst, int32_t* GX, int32_t* GY, int32_t N){
        int32_t rows = src->getHeight();
        int32_t columns = (src->getWidth() * src->getHeight());
        int32_t width = src->getWidth();
        int32_t center = N/2;


        StartTimer(FILTER NO OPT)
        
        for (int32_t row = center; row < ( rows - center ); ++row ) {
                for (int32_t column = center; column < (width - center); ++column ) {
                        double_t gx = 0;
                        double_t gy = 0;

                        for (int32_t i = 0; i < N; ++i ) {
                                for (int32_t j = 0; j < N; ++j ) {
                                        int32_t image_row    = row + i - center;
                                        int32_t image_column = column + j - center;

                                        int32_t index = image_row * width + image_column;

                                        double_t image_value = (double_t)(src->red[index] + src->green[index] + src->blue[index])/3;

                                        int32_t kernel_index = i * N + j;

                                        gx += image_value * GX[kernel_index];
                                        gy += image_value * GY[kernel_index];
                                }
                        }
                        int32_t index = row * width + column;
                        dst->red[index] = dst->green[index] = dst->blue[index] = (sqrt ( gx * gx + gy * gy ));
                        
                }
        }

        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}

void Filters::applyOptimizedKernel(Image* src, Image* dst, int32_t* GX, int32_t* GY, int32_t N){
        int32_t rows = src->getHeight();
        int32_t columns = (src->getWidth() * src->getHeight());
        int32_t width = src->getWidth();
        int32_t center = N/2;
        
        int32_t BLOCK_SIZE = CPU::getBlockSize();


        StartTimer(FILTER OPT)

        for(int32_t row_outer = center; row_outer < (rows - center); row_outer += BLOCK_SIZE){
                for(int32_t col_outer = center; col_outer < (width - center); col_outer += BLOCK_SIZE){
                        for(int32_t row = row_outer; row < (rows - center) && row < (row_outer + BLOCK_SIZE); row++){
                                for(int32_t column = col_outer; column < (columns - center) && column < (col_outer + BLOCK_SIZE); column ++){
                                        double_t gx = 0;
                                        double_t gy = 0;

                                        for (int32_t i = 0; i < N; ++i ) {
                                                for (int32_t j = 0; j < N; ++j ) {
                                                        int32_t image_row    = row + i - center;
                                                        int32_t image_column = column + j - center;
                                                        
                                                        int32_t index = image_row * width + image_column;
                                                        double_t image_value = (double_t)(src->red[index] + src->green[index] + src->blue[index])/3;

                                                        int32_t kernel_index = i * N + j;

                                                        gx += image_value * GX[kernel_index];
                                                        gy += image_value * GY[kernel_index];
                                                }
                                        }
                                        int32_t index = row * width + column;
                                        dst->red[index] = dst->green[index] = dst->blue[index] = (sqrt ( gx * gx + gy * gy ));                
                                }
                        }
                }
        }

        EndTimer
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}