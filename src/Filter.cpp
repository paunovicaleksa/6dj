#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../inc/stb_image.h"
#include "../inc/stb_image_write.h"

#include "../inc/Filter.hpp"


Filter::Filter(){

}

int Filter::load(const char* filename){
        img = stbi_load(filename, &width, &height, &channels, 0);
        if(img == nullptr){
                std::cout << "Error loading image";
                return 1;
        }

        return 0;
}


void Filter::write(){

        stbi_write_jpg("./img/nadeko2.jpg", width, height, channels, img, 100);
}