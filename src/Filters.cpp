#include "../inc/Filters.hpp"
#include "../inc/_Timer.hpp"
#include "../inc/SIMD.hpp"
#include "../inc/CPU.hpp"
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <thread>
#include <sstream>

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

        int32_t thread_num = std::thread::hardware_concurrency();
        std::thread *threads = new std::thread[thread_num];

        int32_t thread_loop_size = loop_size / thread_num;

        auto _grayscale = [&] (int32_t thread_id){
                int32_t loop_start, loop_end;
                loop_start = std::max(0, ((thread_id * thread_loop_size) / 32) * 32);
                loop_end = std::min(loop_size, (((thread_id + 1) * thread_loop_size) / 32) * 32);

                for(int32_t i = loop_start; i < loop_end; i += 32){
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

                        for(int32_t j = 0; j < 4; j++){
                                f_r[j] = _mm256_cvtepi32_ps(dst_r[j]);
                                f_g[j] = _mm256_cvtepi32_ps(dst_g[j]);
                                f_b[j] = _mm256_cvtepi32_ps(dst_b[j]);

                                __m256 sum = _mm256_mul_ps(v_gray, f_r[j]);
                                sum = _mm256_fmadd_ps(v_gray, f_g[j], sum);
                                sum = _mm256_fmadd_ps(v_gray, f_b[j], sum);

                                sum_i[j] = _mm256_cvtps_epi32(sum);
                        }

                        __m256i v_i = _mm256_cvtepi32_epu8(sum_i);
                        _mm256_store_si256 ((__m256i*)&(dst->red[i]),  v_i);
                        _mm256_store_si256 ((__m256i*)&(dst->green[i]),  v_i);
                        _mm256_store_si256 ((__m256i*)&(dst->blue[i]),  v_i);
                }

        };

        for(int32_t id = 0; id < thread_num; id++){
                threads[id] = std::thread(_grayscale, id);
        }

        for(int32_t id = 0; id < thread_num; id++){
                threads[id].join();
        }

        delete[] threads;
 
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
        int32_t mat_center = N/2;


        StartTimer(FILTER NO OPT)
        

        for (int32_t row = mat_center; row < ( rows - mat_center ); ++row ) {
                for (int32_t column = mat_center; column < (width - mat_center); ++column ) {
                        double_t gx = 0;
                        double_t gy = 0;

                        for (int32_t i = 0; i < N; ++i ) {
                                for (int32_t j = 0; j < N; ++j ) {
                                        int32_t image_row    = row + i - mat_center;
                                        int32_t image_column = column + j - mat_center;

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

        StartTimer(FILTER OPT)
        
        int32_t rows = src->getHeight();
        int32_t columns = (src->getWidth() * src->getHeight());
        int32_t width = src->getWidth();
        int32_t mat_center = N/2;
        
        int32_t BLOCK_SIZE = CPU::getBlockSize();

        int32_t thread_num = std::thread::hardware_concurrency();
        int32_t row_size = rows / thread_num;

        std::thread *threads = new std::thread[thread_num];
        
        auto _applyOptimizedKernel = [&](int32_t thread_id) {
                
                int32_t row_start, row_end;
                row_start = std::max(mat_center, (thread_id) * row_size);
                row_end = std::min(rows - mat_center, (thread_id + 1) * row_size);

                for(int32_t row_outer = row_start; row_outer < row_end; row_outer += BLOCK_SIZE){
                        for(int32_t col_outer = mat_center; col_outer < (width - mat_center); col_outer += BLOCK_SIZE){
                                for(int32_t row = row_outer; row < (rows - mat_center) && row < (row_outer + BLOCK_SIZE); row++){
                                        for(int32_t column = col_outer; column < (columns - mat_center) && 
                                                column < (col_outer + BLOCK_SIZE); column ++){

                                                double_t gx = 0;
                                                double_t gy = 0;

                                                for (int32_t i = 0; i < N; ++i ) {
                                                        for (int32_t j = 0; j < N; ++j ) {
                                                                int32_t image_row    = row + i - mat_center;
                                                                int32_t image_column = column + j - mat_center;
                                                                
                                                                int32_t index = image_row * width + image_column;
                                                                double_t image_value = 
                                                                        (double_t)(src->red[index] + src->green[index] + src->blue[index])/3;

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

        };

        for(int32_t id = 0; id < thread_num; id++){
                threads[id] = std::thread(_applyOptimizedKernel, id);
        }

        for(int32_t id = 0; id < thread_num; id++){
                threads[id].join();
        }

        delete[] threads;

        EndTimer
        
        if(src->getChannels() == 4) Image::copyAlpha(src, dst);
}
