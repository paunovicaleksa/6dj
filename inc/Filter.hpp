
#include <unistd.h>
#include <iostream>
#include <string>

class Filter{
public:
        Filter();

        void write();
        int load(const char* filename);

private:
        int32_t width, height, channels;
        uint8_t *img;
};