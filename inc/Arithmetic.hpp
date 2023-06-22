#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include "Image.hpp"

class Arithmetic{
public:
        static void add(Image* src, Image* dst, uint8_t val);
        static void addSIMD(Image* src, Image* dst, uint8_t val);

        static void sub(Image* src, Image* dst, uint8_t val);
        static void subSIMD(Image* src, Image* dst, uint8_t val);
        
        static void invSub(Image* src, Image* dst, uint8_t val);
        static void invSubSIMD(Image* src, Image* dst, uint8_t val);

        static void mul(Image* src, Image* dst, uint8_t val);
        static void mulSIMD(Image* src, Image* dst, uint8_t val);

        static void div(Image* src, Image* dst, uint8_t val);
        static void divSIMD(Image* src, Image* dst, uint8_t val);

        static void invDiv(Image* src, Image* dst, uint8_t val);
        static void invDivSIMD(Image* src, Image* dst, uint8_t val);

};

#endif