#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../inc/stb_image.h"
#include "../inc/stb_image_write.h"
#include "../inc/Filter.hpp"
#include "../inc/_Timer.hpp"
#include "../inc/SIMD.hpp"

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3



inline bool __str_ends_in(std::string str, std::string ends){

        std::uint64_t pos = str.rfind('.');
        std::string sub_str = str.substr(pos, str.length() - 1);

        return sub_str == ends;
}

//step = number of channels, use a loop for readability
void Filter::__init_vector(int color, aligned_vector<uint8_t>& vec){
        vec.reserve(width * height);
        for(int32_t i = color, j = 0; i < size; i += channels, j++){
                vec[j] = img[i];
        }
}
//not using SIMD for this, maybe optimize later?
void Filter::__merge_vectors(){
        for(int32_t i = 0, j = 0; i < size; i += channels, j++){
                img[i + 0] = red[j];
                img[i + 1] = green[j];
                img[i + 2] = blue[j];
                
                if(channels == 4) img[i + 3] = alpha[j];
        }
}

Filter::Filter(const char* f_name): file_name(f_name){

}

Filter::~Filter(){
        stbi_image_free(img);
}

int Filter::load(){
        img = stbi_load(file_name.c_str(), &width, &height, &channels, 0);
        if(img == nullptr){
                std::cout << "Error loading image";
                return 1;
        }
        size = width * height * channels;

        std::cout << channels << std::endl;

        __init_vector(RED, red);
        __init_vector(GREEN, green);
        __init_vector(BLUE, blue);

        if(channels == 4) __init_vector(ALPHA, alpha);


        return 0;
}

void Filter::add(uint8_t val){

        StartTimer(ADD NO SIMD)

        for(int32_t i = 0; i < width * height; i++){
                red[i] += val;
                green[i] += val;
                blue[i] += val;
        }

        EndTimer

}

void Filter::addSIMD(uint8_t val){
        StartTimer(ADD SIMD)

        int32_t loop_size = ((width * height) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&red[i]);

                __m256i v_green = _mm256_loadu_si256((__m256i*)&green[i]);

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&blue[i]);
               
                v_red = _mm256_add_epi8(v_red, v_val);
                v_green = _mm256_add_epi8(v_green, v_val);
                v_blue = _mm256_add_epi8(v_blue, v_val);

                _mm256_store_si256 ((__m256i*)&red[i],  v_red);
                _mm256_store_si256 ((__m256i*)&green[i],  v_green);
                _mm256_store_si256 ((__m256i*)&blue[i],  v_blue);
        }

        for(int32_t i = loop_size; i < width*height; i++){
                red[i] += val;
                green[i] += val;
                blue[i] += val;
        }
        


        EndTimer
        
}

void Filter::sub(uint8_t val){

        StartTimer(SUB NO SIMD)

        for(int32_t i = 0; i < width * height; i++){
                red[i] -= val;
                green[i] -= val;
                blue[i] -= val;
        }

        EndTimer
}

void Filter::subSIMD(uint8_t val){
        StartTimer(SUB SIMD)

        int32_t loop_size = ((width * height) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&red[i]);

                __m256i v_green = _mm256_loadu_si256((__m256i*)&green[i]);

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&blue[i]);
               
                v_red = _mm256_sub_epi8(v_red, v_val);
                v_green = _mm256_sub_epi8(v_green, v_val);
                v_blue = _mm256_sub_epi8(v_blue, v_val);

                _mm256_store_si256 ((__m256i*)&red[i],  v_red);
                _mm256_store_si256 ((__m256i*)&green[i],  v_green);
                _mm256_store_si256 ((__m256i*)&blue[i],  v_blue);
        }

        for(int32_t i = loop_size; i < width*height; i++){
                red[i] -= val;
                green[i] -= val;
                blue[i] -= val;
        }
        

        EndTimer
}

void Filter::mul(uint8_t val){
        StartTimer(MUL NO SIMD)

        for(int32_t i = 0; i < width * height; i++){
                red[i] *= val;
                green[i] *= val;
                blue[i] *= val;
        }

        EndTimer

}

void Filter::invSub(uint8_t val){
        StartTimer(INV SUB NO SIMD)

        for(int32_t i = 0; i < width * height; i++){
                red[i] = val - red[i];
                green[i] = val - green[i];
                blue[i] = val - blue[i];
        }

        EndTimer

}
void Filter::invSubSIMD(uint8_t val){
        StartTimer(INV SUB SIMD)

        int32_t loop_size = ((width * height) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&red[i]);

                __m256i v_green = _mm256_loadu_si256((__m256i*)&green[i]);

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&blue[i]);
               
                v_red = _mm256_sub_epi8( v_val, v_red);
                v_green = _mm256_sub_epi8(v_val, v_green);
                v_blue = _mm256_sub_epi8(v_val, v_blue);

                _mm256_store_si256 ((__m256i*)&red[i],  v_red);
                _mm256_store_si256 ((__m256i*)&green[i],  v_green);
                _mm256_store_si256 ((__m256i*)&blue[i],  v_blue);
        }

        for(int32_t i = loop_size; i < width*height; i++){
                red[i] = val - red[i];
                green[i] = val - green[i];
                blue[i] = val - blue[i];
        }
        

        EndTimer

}

void Filter::mulSIMD(uint8_t val){
        StartTimer(MUL SIMD)

        int32_t loop_size = ((width * height) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&red[i]);

                __m256i v_green = _mm256_loadu_si256((__m256i*)&green[i]);

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&blue[i]);
               
                v_red = _mm256_mullo_epi8( v_val, v_red);
                v_green = _mm256_mullo_epi8(v_val, v_green);
                v_blue = _mm256_mullo_epi8(v_val, v_blue);

                _mm256_store_si256 ((__m256i*)&red[i],  v_red);
                _mm256_store_si256 ((__m256i*)&green[i],  v_green);
                _mm256_store_si256 ((__m256i*)&blue[i],  v_blue);
        }

        for(int32_t i = loop_size; i < width*height; i++){
                red[i] *= val;
                green[i] *= val;
                blue[i] *= val;
        }
        

        EndTimer

}

void Filter::div(uint8_t val){
        StartTimer(DIV NO SIMD)

        for(int32_t i = 0; i < width * height; i++){
                red[i] /= val;
                green[i] /= val;
                blue[i] /= val;
        }

        EndTimer
}

void Filter::divSIMD(uint8_t val){
        StartTimer(DIV SIMD)

        int32_t loop_size = ((width * height) / 32 ) * 32;

        __m256i v_val =  _mm256_set1_epi8(val); //sets 32 members of vval to val

        for(int32_t i = 0; i < loop_size; i += 32){
               
                __m256i v_red = _mm256_loadu_si256((__m256i*)&red[i]);

                __m256i v_green = _mm256_loadu_si256((__m256i*)&green[i]);

                __m256i v_blue =_mm256_loadu_si256((__m256i*)&blue[i]);
               
                v_red = _mm256_div_epi8 (v_red, v_val);
                v_green = _mm256_div_epi8(v_green, v_val);
                v_blue = _mm256_div_epi8(v_red, v_val);

                _mm256_store_si256 ((__m256i*)&red[i],  v_red);
                _mm256_store_si256 ((__m256i*)&green[i],  v_green);
                _mm256_store_si256 ((__m256i*)&blue[i],  v_blue);
        }

        for(int32_t i = loop_size; i < width*height; i++){
                red[i] /= val;
                green[i] /= val;
                blue[i] /= val;
        }
        

        EndTimer
}


void Filter::write(){
        
        __merge_vectors();

        std::uint64_t pos = file_name.rfind('.');
        std::string write_name = file_name.substr(0, pos) + "_modified"; 

        if(__str_ends_in(file_name.c_str(), ".jpg") || __str_ends_in(file_name.c_str(), ".JPG") || 
           __str_ends_in(file_name.c_str(), ".jpeg") || __str_ends_in(file_name.c_str(), ".JPEG")) {
                write_name += ".jpg";
                stbi_write_jpg(write_name.c_str(), width, height, channels, img, 100);

        } else if(__str_ends_in(file_name.c_str(), ".png") || __str_ends_in(file_name.c_str(), ".PNG")) {
                write_name += ".png";
                stbi_write_png(write_name.c_str(), width, height, channels, img, width * channels);
        } 
}

