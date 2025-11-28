#include <iostream>
#include <cmath>

class vec {
private:
    float x , y , z;
public:
    explicit vec(float x=0 , float y=0 , float z=0) : x{x} , y{y} , z{z} {}
    vec(const vec &v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    [[nodiscard]] float getX() const { return x; }
    [[nodiscard]] float getY() const { return y; }
    [[nodiscard]] float getZ() const { return z; }
    void setX(float a) { x = a; }
    void setY(float b) { y = b; }
    void setZ(float c) { z = c; }
    [[nodiscard]] float modul() const {
        return sqrtf(x * x + y * y + z * z);
    }

    vec& operator=(const vec &v) = default;
    vec operator+(const vec &v) const {
        return vec(this->x+v.x, this->y+v.y, this->z+v.z);
    }
    vec operator+(int) {
        vec temp = *this;
        x++ ; y++ ; z++;
        return temp;
    }
    vec operator--(int) {
        vec temp = *this;
        x-- ; y-- ; z--;
        return temp;
    }
    vec operator-(const vec &v) const {
        return vec(this->x-v.x, this->y-v.y, this->z-v.z);
    }
    vec operator*(float scalar) const {
        return vec(scalar*x , scalar*y , scalar*z);
    }
    vec& operator+=(const vec &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    vec& operator-=(const vec &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    vec& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    bool operator==(const vec &v) const {
        return this->modul() == v.modul();
    }
    bool operator!=(const vec &v) const{
        return this->modul() != v.modul();
    }
    bool operator<(const vec &v) const {
        return this->modul() < v.modul();
    }
    bool operator>(const vec &v) const {
        return this->modul() > v.modul();
    }
    bool operator<=(const vec &v) const {
        return this->modul() <= v.modul();
    }
    bool operator>=(const vec &v) const {
        return this->modul() >= v.modul();
    }
    float operator~() const {
        return modul();
    }
    vec& operator++() {
        this->x++;
        this->y++;
        this->z++;
        return *this;
    }
    vec& operator--() {
        this->x--;
        this->y--;
        this->z--;
        return *this;
    }
    vec& operator+() {
        return *this;
    }
    vec operator-() const {
        return vec(-x , -y , -z );
    }
    float operator[](int index) const {
        if (index==0) return this->x;
        if (index==1) return this->y;
        if (index==2) return this->z;
        throw std::out_of_range("Index must be 0, 1, or 2");
    }
};

std::ostream& operator<<(std::ostream& os, const vec& v) {
    os << "x: " << v.getX() << ", y: " << v.getY() << ", z: " << v.getZ() ;
    return os;
}
std::istream& operator>>(std::istream& is, vec& v) {
    float a , b , c;
    is >> a >> b >> c ;
    v.setX(a);
    v.setY(b);
    v.setZ(c);
    return is;
}

int main() {
    vec v1(1.0f, 2.0f, 3.0f);
    vec v2(4.0f, 5.0f, 6.0f);

    std::cout << "v1: " << v1 << "\n";
    std::cout << "v2: " << v2 << "\n";

    vec v3 = v1 + v2;
    vec v4 = v2 - v1;
    vec v5 = v1 * 2.0f;

    std::cout << "v1 + v2 = " << v3 << "\n";
    std::cout << "v2 - v1 = " << v4 << "\n";
    std::cout << "v1 * 2 = " << v5 << "\n";

    v1 += v2;
    std::cout << "v1 += v2: " << v1 << "\n";

    v2 -= vec(1,1,1);
    std::cout << "v2 -= (1,1,1): " << v2 << "\n";

    std::cout << "Unary -v2: " << -v2 << "\n";
    ++v1;
    std::cout << "++v1: " << v1 << "\n";
    v1--;
    std::cout << "v1--: " << v1 << "\n";

    std::cout << "Magnitude of v1: " << ~v1 << "\n";

    vec a(1,2,3);
    vec b(1,2,3);
    std::cout << "a == b: " << (a == b) << "\n";
    std::cout << "a != b: " << (a != b) << "\n";

    std::cout << "v1[0] = " << v1[0] << "\n";

    vec v_input;
    std::cout << "Enter 3 floats for a vector: ";
    std::cin >> v_input;
    std::cout << "You entered: " << v_input << "\n";

    return 0;
}