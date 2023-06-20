#include <iostream>
#include "../inc/Filter.hpp"


int main(){ 

        Filter *f = new Filter("./img/leave.jpg");
        if(f->load()){
                exit(1);
        }
        //f->add(200);
        f->grayscaleSIMD();
        f->write();
        
        int a;
        std::cout << "coninue? ";
        std::cin >> a;

        //f->min(100);
        //f->minSIMD(100);
        //f->write();

        delete f;
}