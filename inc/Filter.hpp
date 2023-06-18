
#include <unistd.h>
#include <iostream>
#include <string>
#include <immintrin.h>
#include <vector>

class Filter{
public:
        Filter(const char* f_name);
        ~Filter();
        void write();
        int load();

        void add(uint8_t);
        void addSIMD(uint8_t);

private:
        int32_t width, height, channels;
        int32_t size;
        std::string filename;
        uint8_t *img;
};      