#ifndef FILTERS_HPP
#define FILTERS_HPP

#include "Image.hpp"

class Filters{
public:
        static void invert(Image* src, Image* dst);
        static void invertSIMD(Image* src, Image* dst);

        static void grayscale(Image* src, Image* dst);
        static void grayscaleSIMD(Image* src, Image* dst); 

        static void applyKernel(Image* src, Image* dst, int32_t* GX, int32_t* GY, int32_t N);
        static void applyOptimizedKernel(Image* src, Image* dst, int32_t* GX, int32_t* GY, int32_t N);

};

#endif