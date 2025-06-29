#pragma once
#include <iostream>

class Vector2D
{
public:
    float x;
    float y;

    Vector2D();
    Vector2D(float x, float y);

    Vector2D& Add(const Vector2D& vec);
    Vector2D& Subtract(const Vector2D& vec);
    Vector2D& Multiply(const Vector2D& vec);
    Vector2D& Divide(const Vector2D& vec);

    friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D operator*(const Vector2D& v1, const Vector2D& v2);
    friend Vector2D operator/(const Vector2D& v1, const Vector2D& v2);

    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const Vector2D& vec);
    Vector2D& operator/=(const Vector2D& vec);

    Vector2D& operator*(const int& i);
    Vector2D& Zero();

    // returns a new Vector2D 
    Vector2D getNormalised() const; 

    // rotates vector acw by r radians
    Vector2D& rotateACW(float r);
    Vector2D& rotateCW(float r);

    // scales vector by scale factor
    Vector2D& scale(float scale);

    float magnitude() const { return std::sqrt(std::powf(x, 2) + std::powf(y, 2)); }    // check float, something is off with normal

    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};