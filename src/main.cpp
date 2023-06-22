#include <iostream>
#include "../inc/Parser.hpp"
#include "../inc/Arithmetic.hpp"
#include "../inc/MathFunc.hpp"
#include "../inc/Filters.hpp"

int main(int argc, char* argv[]){

        if(argc < 2){
                std::cout << "this requires at least some arguments!" << std::endl;
                exit(1);
        }
        Parser::parse(argc, argv);
     


        return 0;
}