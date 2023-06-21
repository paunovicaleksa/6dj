#include <iostream>
#include "../inc/Filter.hpp"
#include "../inc/CPU.hpp"

int main(){ 
        std::cout << CPU::getBlockSize() << std::endl;
        int GX[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
        int GY[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
        
        Filter *f = new Filter("./img/sofija1.png");
        if(f->load()){
                exit(1);
        }
        f->filterOptimized(GX, GY);
        //f->min(100);
        //f->minSIMD(100);
        //f->write();

}