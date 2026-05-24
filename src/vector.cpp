#include "vector.h"

namespace PHYSX {

    MyVector MyVector::operator+(const MyVector v) {
        return MyVector(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    void MyVector::operator+=(const MyVector v) {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
    }

    MyVector MyVector::operator-(const MyVector v) {
        return MyVector(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    void MyVector::operator-=(const MyVector v) {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
    }

    MyVector MyVector::operator*(const float s) {
        return MyVector(this->x * s, this->y * s, this->z * s);
    }

    MyVector operator*(const float s, MyVector const& v) {
        return MyVector(v.x * s, v.y * s, v.z * s);
    }

    void MyVector::operator*=(const float s) {
        this->x *= s;
        this->y *= s;
        this->z *= s;
    }

    MyVector MyVector::operator*(MyVector const& v) {
        return MyVector(this->x * v.x, this->y * v.y, this->z * v.z);
    }

    MyVector MyVector::Cross(MyVector const& v) {
        return MyVector(
            this->y * v.z - this->z * v.y,
            this->z * v.x - this->x * v.z,
            this->x * v.y - this->y * v.x
        );
    }

    float MyVector::Dot(MyVector const& v) {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    float MyVector::Magnitude() {
        return std::sqrt(x * x + y * y + z * z);
    }

    MyVector MyVector::Normalize() {
        float mag = Magnitude();
        if (mag == 0.0f) return MyVector(0.0f, 0.0f, 0.0f);
        return MyVector(x / mag, y / mag, z / mag);
    }

    void MyVector::Print() {
        std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }

}