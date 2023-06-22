#ifndef MATHFUNC_HPP
#define MATHFUNC_HPP

#include "Image.hpp"

class MathFunc{
public:
        static void min(Image* src, Image* dst, uint8_t val);
        static void minSIMD(Image* src, Image* dst, uint8_t val);

        static void max(Image* src, Image* dst, uint8_t val);
        static void maxSIMD(Image* src, Image* dst, uint8_t val);

        static void log(Image* src, Image* dst);
        static void logSIMD(Image* src, Image* dst);

        static void abs(Image* src, Image* dst);
        static void absSIMD(Image* src, Image* dst);

        static void pow(Image* src, Image* dst, _Float32 val);
        static void powSIMD(Image* src, Image* dst, _Float32 val);      
};

#endif