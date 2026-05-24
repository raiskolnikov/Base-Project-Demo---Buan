#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>

namespace PHYSX {
    class MyVector {
        public:
            float x, y, z;

            MyVector() : x(0), y(0), z(0) {}

            MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

            MyVector operator+ (const MyVector v);
            void operator+= (const MyVector v);

            MyVector operator-(const MyVector v);
            void operator-=(const MyVector v);

            MyVector operator*(const float s);
            friend MyVector operator*(const float s, MyVector const& v);
            void operator*=(const float s);

            float Magnitude();
            MyVector Normalize();

            MyVector operator*(MyVector const &v);
            MyVector Cross(MyVector const &v);
            float Dot(MyVector const &v);

            void Print();
    };
}
    
