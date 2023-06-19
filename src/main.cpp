#include <iostream>
#include "../inc/Filter.hpp"


int main(){ 

        Filter *f = new Filter("./img/bg1.png");
        if(f->load()){
                exit(1);
        }
        f->add(200);
        f->addSIMD(200);
        f->write();
        
        int a;
        std::cout << "coninue? ";
        std::cin >> a;

        f->invSub(200);
        f->invSubSIMD(200);
        f->write();

        delete f;
}