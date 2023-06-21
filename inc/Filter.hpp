
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/align/aligned_allocator.hpp>

#ifndef FILTER_HPP
#define FILTER_HPP

template <typename T>
using aligned_vector = std::vector<T, boost::alignment::aligned_allocator<T, 32>>;

class Filter{
public:
        Filter(const char* f_name);
        ~Filter();
        void write();
        int load();

        void add(uint8_t);
        void addSIMD(uint8_t);

        void sub(uint8_t);
        void subSIMD(uint8_t);
        
        void invSub(uint8_t);
        void invSubSIMD(uint8_t);

        void mul(uint8_t);
        void mulSIMD(uint8_t);

        void div(uint8_t);
        void divSIMD(uint8_t);

        void invDiv(uint8_t);
        void invDivSIMD(uint8_t);

        void min(uint8_t);
        void minSIMD(uint8_t);

        void max(uint8_t);
        void maxSIMD(uint8_t);

        void log();
        void logSIMD();

        void abs();
        void absSIMD();

        void invert();
        void invertSIMD();

        void grayscale();
        void grayscaleSIMD();
        
        void pow(int32_t);
        void powSIMD(int32_t);

        void filter(int32_t* GX, int32_t* GY);
        void filterOptimized(int32_t* GX, int32_t* GY);

protected:
        void __init_vector(int color, aligned_vector<uint8_t>& vec);
        void __merge_vectors();

private:
        int32_t width, height, channels;
        int32_t size;
        std::string file_name;
        uint8_t *img;
        aligned_vector<uint8_t> red, green, blue;
        aligned_vector<uint8_t> alpha;
};      

#endif