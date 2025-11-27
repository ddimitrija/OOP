#include "../include/MyVec.h"
#include <iostream>
MyVec::MyVec(int size, int value) {
    int capacity = 4;
    while (size >= capacity) {
        capacity*=2;
    }
    elements = new int[capacity];
    this->capacity = capacity;
    this->size = size;

    for (int i=0; i<this->size;i++ ) {
        elements[i] = value;
    }
}

MyVec::~MyVec() {
    delete [] elements;
}

void MyVec::_copy(const MyVec& orig) {
    this->capacity = orig.capacity;
    this->size = orig.size;
    this->elements = new int[this->capacity];
    for (int i=0; i<this->size; i++) {
        this->elements[i] = orig.elements[i];
    }
}

MyVec::MyVec(const MyVec& orig) {
    _copy(orig);
}

MyVec& MyVec::operator=(const MyVec& orig) {
    if (this != &orig) {
        delete [] this->elements;
        _copy(orig);
    }
    return *this;
}

void MyVec::print() {
    std::cout<<"C: "<< this->capacity << " S: " << this->size <<":\n";
    for (int i=0; i<this->size; i++) {
        std::cout<<this->elements[i] << " ";
    }
    std::cout<<std::endl;
}

void MyVec::push(int val) {
   if (this->size == this->capacity) {
        int newcap = this->capacity*2;
        int *tmp = new int[newcap];
        for (int i=0; i<this->size; i++) {
            tmp[i] = this->elements[i];
        }
        delete [] elements;
        elements = tmp;
        this->capacity = newcap;
    }

    this->elements[this->size] = val;
    this->size++;
}

void MyVec::pop() {
    if (this->size > 0) {
        this->size--;
        if (this->size<this->capacity/2 && this->capacity>4) {
            int newCap=this->capacity/2;
            int* temp=new int[newCap];
            for (int i=0;i<this->size;i++) {
                temp[i]=this->elements[i];
            }
            delete[] this->elements;
            this->elements=temp;
            this->capacity=newCap;
        }
    }
}

int MyVec::operator[](int i) {
    if (i<0 || i > this->size-1) {
        return 42;
    }
    return this->elements[i];
}

void MyVec::sort() {
    for (int i=0;i<this->size - 1; i++) {
        for (int j=0;j< this->size - i - 1;j++) {
            if (this->elements[j]>this->elements[j+1]) {
                int temp=this->elements[j];
                this->elements[j]=this->elements[j+1];
                this->elements[j+1]=temp;
            }
        }
    }
}

int MyVec::back() const {
    return this->elements[size-1];
}

int MyVec::front() const {
    return this->elements[0];
}

int MyVec::at(const int pos) const {
    if (pos>=size) {
        return -1;
    }
    return this->elements[pos];
}

int* MyVec::data() const{
    return this->elements;
}

MyVec MyVec::operator+(const MyVec& orig) {
    MyVec newVec(this->size + orig.size);
    for (int i=0;i<this->size;i++) {
        newVec.elements[i]=this->elements[i];
    }
    for (int i=0;i<orig.size ; i++) {
        newVec.elements[this->size+i]=orig.elements[i];
    }
    return newVec;
}


