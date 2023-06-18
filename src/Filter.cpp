#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../inc/stb_image.h"
#include "../inc/stb_image_write.h"
#include "../inc/Filter.hpp"

inline bool str_ends_in(std::string str, std::string ends) {

        std::uint64_t pos = str.rfind('.');
        std::string sub_str = str.substr(pos, str.length() - 1);

        return sub_str == ends;
}



Filter::Filter(const char* f_name): filename(f_name){

}

Filter::~Filter(){
        stbi_image_free(img);
}

int Filter::load(){
        img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if(img == nullptr){
                std::cout << "Error loading image";
                return 1;
        }
        size = width * height * channels;
        
        return 0;
        }

void Filter::add(uint8_t val){
        for(int i = 0; i < size; i++){
                img[i] += val;
        }

}

void Filter::write(){


        if(str_ends_in(filename.c_str(), ".jpg") || str_ends_in(filename.c_str(), ".JPG") || 
           str_ends_in(filename.c_str(), ".jpeg") || str_ends_in(filename.c_str(), ".JPEG")) {
                
                stbi_write_jpg(filename.c_str(), width, height, channels, img, 100);

        } else if(str_ends_in(filename.c_str(), ".png") || str_ends_in(filename.c_str(), ".PNG")) {

                stbi_write_png(filename.c_str(), width, height, channels, img, width * channels);
        } 
}

