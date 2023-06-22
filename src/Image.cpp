#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../inc/stb_image.h"
#include "../inc/stb_image_write.h"
#include "../inc/Image.hpp"

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

void Image::copyAlpha(Image* src, Image* dst){
        for (int32_t i = 0; i < (src->getWidth() * src->getHeight()); i++){
                dst->alpha[i] = src->alpha[i];
        }    
}

inline bool __str_ends_in(std::string str, std::string ends){

        std::uint64_t pos = str.rfind('.');
        std::string sub_str = str.substr(pos, str.length() - 1);

        return sub_str == ends;
}

 void Image::__init_vector(int color, aligned_vector<uint8_t>& vec){
      
        vec.reserve(width * height);
        for(int32_t i = color, j = 0; i < size; i += channels, j++){
                vec[j] = img[i];
        }
}

void Image::__merge_vectors(){

         for(int32_t i = 0, j = 0; i < size; i += channels, j++){
                img[i + 0] = red[j];
                img[i + 1] = green[j];
                img[i + 2] = blue[j];
                
                if(channels == 4) img[i + 3] = alpha[j];
        }
}

Image::Image(const char* f_name): file_name(f_name), write_only(false){}

Image::Image(const char* f_name, int32_t w, int32_t h, int32_t c): file_name(f_name), 
        width(w), height(h), write_only(true){
                channels = c;
                img = new uint8_t[width * height * channels];   
                
                size = width * height * channels;

                red.reserve(width * height);
                blue.reserve(width * height);
                green.reserve(width * height);

                if(channels == 4) alpha.reserve(width * height);
}

Image::~Image(){
        if(write_only){
                delete[] img;
        } else {
                stbi_image_free(img);
        }
}
int Image::load(){

        if(write_only) return 1;

        img = stbi_load(file_name.c_str(), &width, &height, &channels, 0);
        if(img == nullptr){
                std::cout << "Error loading image";
                return 1;
        }

        size = width * height * channels;

        __init_vector(RED, red);
        __init_vector(GREEN, green);
        __init_vector(BLUE, blue);

        if(channels == 4) __init_vector(ALPHA, alpha);

        return 0;
}
void Image::write(){
        
        __merge_vectors();

        std::uint64_t pos = file_name.rfind('.');
        std::string write_name = file_name.substr(0, pos) + "_modified"; 

        std::cout << "writing image-";

        if(__str_ends_in(file_name.c_str(), ".jpg") || __str_ends_in(file_name.c_str(), ".JPG") || 
           __str_ends_in(file_name.c_str(), ".jpeg") || __str_ends_in(file_name.c_str(), ".JPEG")) {
                write_name += ".jpg";
                stbi_write_jpg(write_name.c_str(), width, height, channels, img, 100);

        } else if(__str_ends_in(file_name.c_str(), ".png") || __str_ends_in(file_name.c_str(), ".PNG")) {
                write_name += ".png";
                stbi_write_png(write_name.c_str(), width, height, channels, img, width * channels);
        } else{
                std::cout << "failed" << std::endl;
                return;
        }

        std::cout << "done" << std::endl;
}
