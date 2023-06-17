#include <iostream>
#include "../inc/Filter.hpp"


int main(){ 

        Filter *f = new Filter();
        f->load("./img/nadeko.jpg");
        f->write();
}