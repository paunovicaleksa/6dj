#include <iostream>
#include "../inc/Filter.hpp"


int main(){ 

        Filter *f = new Filter("./img/bg1.png");
        if(f->load()){
                exit(1);
        }
        f->mul(200);
        f->mulSIMD(200);
        f->write();
        
        int a;
        std::cout << "coninue? ";
        std::cin >> a;

        //f->div(2);
        f->divSIMD(2);
        f->write();

        delete f;
}