#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/align/aligned_allocator.hpp>


template <typename T>
using aligned_vector = std::vector<T, boost::alignment::aligned_allocator<T, 32>>;


class Image{
public:
        /* constructor for reading mostly */
        Image(const char* f_name);


        /* constructor for writing ONLY*/
        Image(const char* f_name, int32_t w, int32_t h, int32_t c);
        ~Image();

        void write();
        int load();

        std::string getFileName() const {return file_name;}
        int32_t getWidth() const {return width;}
        int32_t getHeight() const {return height;}
        int32_t getChannels() const {return channels;}

        /* cant copy vectors normally for some reason, fix later maybe?? */
        static void copyAlpha(Image* src, Image* dst);

        /* public for image editing */
        aligned_vector<uint8_t> red, green, blue;
        aligned_vector<uint8_t> alpha;

protected:
        void __init_vector(int color, aligned_vector<uint8_t>& vec);
        void __merge_vectors();

private:
        bool write_only;
        int32_t width, height, channels;
        int32_t size;
        std::string file_name;
        uint8_t *img;
};




#endif