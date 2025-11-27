#ifndef SRC_MYVEC_H
#define SRC_MYVEC_H

class MyVec {
private:
    int size;
    int capacity;
    int *elements;

    void _copy(const MyVec& orig);
public:
    MyVec(int size = 0, int value = 0);
    ~MyVec();
    MyVec(const MyVec& orig);
    MyVec& operator=(const MyVec& orig);
    void print();
    void push(int val);
    void pop();
    int front() const;
    int back() const;
    void sort();
    int at(int pos) const;
    int* data() const;
    MyVec operator+(const MyVec& orig);

    int operator[](int i);
};
#endif //SRC_MYVEC_H