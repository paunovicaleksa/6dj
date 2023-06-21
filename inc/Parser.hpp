#include <getopt.h>
#include <string>
#include <unistd.h>
#include "Filter.hpp"

#ifndef PARSER_HPP
#define PARSER_HPP



class Parser{
public:    
        static void printHelp();
        static int parse(int32_t argc, char* argv[]);
        /* TODO: implement tests */        
        static void test(std::string test_name, Filter* f);

};

#endif