#include <iostream>
#include "../inc/Filter.hpp"
#include "../inc/CPU.hpp"
#include "../inc/Parser.hpp"
#include <getopt.h>

int main(int argc, char* argv[]){

        if(argc < 2){
                std::cout << "this requires at least some arguments!" << std::endl;
                exit(1);
        }
        Parser::parse(argc, argv);

        return 0;
}