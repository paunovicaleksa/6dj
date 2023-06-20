#include <iostream>
#include "../inc/Filter.hpp"


int main(){ 

        Filter *f = new Filter("./img/bg1.png");
        if(f->load()){
                exit(1);
        }
        f->addSIMD(200);
        //f->log();
        f->write();
        
        int a;
        std::cout << "coninue? ";
        std::cin >> a;

        //f->min(100);
        //f->minSIMD(100);
        //f->write();

        delete f;
}