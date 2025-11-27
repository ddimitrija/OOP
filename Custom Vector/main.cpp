#include "include/MyVec.h"
#include<iostream>

int main() {
    MyVec v(3, 123);
    v.print();

    v.push(999);
    v.push(777);
    v.pop();
    v.push(777);
    v.push(333);
    v.push(122);
    v.push(10112554);

    v.print();
    std::cout << "Posleden element (back()) : " << v.back() << std::endl;
    std::cout << "Prv element (front()) : " << v.front() << std::endl;
    std::cout << "Elementot na pozicija 5 e : " << v.at(5) << std::endl;
    MyVec add=v+v;
    add.print();    // Proverka na operator+
    add.pop();
    add.print();    //Proveruva dali raboti namaluvanje na kapacitet
}

