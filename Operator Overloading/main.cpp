#include<iostream>
using namespace std;

// +, -, *, /, ==, <, >
class Frac {
    int n;
    int d;
public:
    int N () const { return this->n; }
    int D () const { return this->d; }
    void setN(int x) { this->n = x; }
    void setD(int x) { this->d = x; }

    Frac(const int n, const int d=1) {
        this->n = n;
        this->d = d;
    }

    Frac (const Frac& orig) {
        this->n = orig.n;
        this->d = orig.d;
    }

    Frac& operator=(const Frac& orig) {
        if (this!=&orig) { // No Self-assignment
            this->n = orig.n;
            this->d = orig.d;
        }
        return *this;
    }

    bool operator==(const Frac& orig) const{
        if (this->n == orig.n , this->d == orig.d) {
            return true;
        }
        return false;
    }

    Frac operator/(const Frac& orig) const {
        Frac c(1);
        c.setN(this->n * orig.d);
        c.setD(this->d * orig.n);
        return c;
    }

    bool operator<(const Frac& orig) const{
        float d1=this->n / this->d;
        float d2=orig.n / orig.d;
        if (d1<d2) { return true; }
        return false;
    }

    bool operator>(const Frac& orig) const{
        float d1=this->n / this->d;
        float d2=orig.n / orig.d;
        if (d1<d2) { return false; }
        return true;
    }

    Frac operator+(const Frac& orig) const {
        Frac c(1);
        c.setN(this->n * orig.d + this->d * orig.n);
        c.setD(this->d * orig.d);
        return c;
    }

    Frac operator-(const Frac& orig) const {
        Frac c(1);
        c.setN(this->n * orig.d - this->d * orig.n);
        c.setD(this->d * orig.d);
        return c;
    }

    Frac operator*(const Frac& b) const {
        Frac c(*this);
        c.setN(this->N() * b.N());
        c.setD(this->D() * b.D());
        return c;
    }

    void print() const {
        cout<<this->n <<"/" << this->d;
    }

    ~Frac() {}

};

int main() {
    Frac a(1 , 3);
    Frac b(2 , 4);
    Frac add=a+b;
    Frac sub=a-b;
    Frac mul=a*b;
    Frac div=a/b;
    if (a>b) { cout << "a is larger" << endl;}
    if (a<b) { cout << "b is larger" << endl; }
    if (a==b) {cout << "a is equal to b" << endl; }
    cout << "addition : " ;
    add.print();  cout << endl;
    cout << "subtraction : " ;
    sub.print() ; cout << endl;
    cout << "multiplication : " ;
    mul.print();  cout << endl;
    cout << "division : " ;
    div.print();  cout << endl;

    return 0;
}