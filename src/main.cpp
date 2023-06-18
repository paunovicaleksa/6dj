#include <iostream>
#include "../inc/Filter.hpp"


int main(){ 

        Filter *f = new Filter("./img/sofija2.jpg");
        if(f->load()){
                exit(1);
        }
        f->add(50);
        f->write();
        delete f;
}