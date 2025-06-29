#include "Vector2D.h"

Vector2D::Vector2D()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D& Vector2D::Add(const Vector2D& vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this; 
}

Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this; 
}

Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
    this->x *= vec.x;
    this->y *= vec.y;
    return *this; 
}

Vector2D& Vector2D::Divide(const Vector2D& vec)
{
    this->x /= vec.x;
    this->y /= vec.y;
    return *this; 
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D result = v1;
    result.Add(v2);
    return result;
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D result = v1;
    result.Subtract(v2);
    return result;
}

Vector2D operator*(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D result = v1;
    result.Multiply(v2);
    return result;
}

Vector2D operator/(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D result = v1;
    result.Divide(v2);
    return result;
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
    return this->Add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
    return this->Subtract(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
    return this->Multiply(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
    return this->Divide(vec);
}

Vector2D& Vector2D::operator*(const int& i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2D& Vector2D::Zero()
{
    this->x = 0;
    this->y = 0;
    return *this;
}

Vector2D Vector2D::getNormalised() const
{
    float mag = this->magnitude();
    if (mag > 0) { return Vector2D(x/mag, y/mag); }
    return Vector2D(0.0f, 0.0f);
}

Vector2D& Vector2D::scale(float scale)
{
    this->x *= scale;
    this->y *= scale;
    return *this;
}

// rotates anti-clockwise by r radians
Vector2D& Vector2D::rotateACW(float r)
{
    float X = x * cos(r) - y * sin(r);  // linear alg coming in clutch
    float Y = x * sin(r) + y * cos(r);

    this->x = X;
    this->y = Y;
    return *this;
}

// rotates clockwise by r radians; will people in the future not know what clockwise is, if all clocks become digital?
Vector2D& Vector2D::rotateCW(float r)
{
    float X = x * cos(r) + y * sin(r);
    float Y = y * cos(r) - x * sin(r);

    this->x = X;
    this->y = Y;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
    stream << "(" << vec.x << "," << vec.y << ")";
    return stream;
}