/******************************************************************************/
/*!
\file	Vector2.cpp
\author Foo Jing Ting
\par	email: 152856H\@nyp.edu.sg
\brief
Struct to define a 2D vector
*/
/******************************************************************************/

#include <cmath>
#include "Vector2.h"

/******************************************************************************/
/*!
\brief
Vector2 default constructor

\param  a
x value
\param  b
y value
*/
/******************************************************************************/
Vector2::Vector2(float a, float b) : x(a), y(b)
{
}

/******************************************************************************/
/*!
\brief
Vector2 copy constructor

\param  rhs
Vector to copy from
*/
/******************************************************************************/
Vector2::Vector2(const Vector2& rhs) : x(rhs.x), y(rhs.y)
{
}

/******************************************************************************/
/*!
\brief
Set the elements of this vector

\param  a
x value
\param  b
y value
*/
/******************************************************************************/
void Vector2::Set(float a, float b)
{
    x = a;
    y = b;
}

/******************************************************************************/
/*!
\brief
operator+ overload for vector addition

\param  rhs
Vector2 to add with
\return
Resulting vector
*/
/******************************************************************************/
Vector2 Vector2::operator+(const Vector2& rhs) const
{
    return Vector2(x + rhs.x, y + rhs.y);
}

/******************************************************************************/
/*!
\brief
operator- overload for vector subtraction

\param  rhs
Vector2 to subtract with
\return
Resulting vector
*/
/******************************************************************************/
Vector2 Vector2::operator-(const Vector2& rhs) const
{
    return Vector2(x - rhs.x, y - rhs.y);
}

/******************************************************************************/
/*!
\brief
operator- overload for unary negation

\return
Resulting vector
*/
/******************************************************************************/
Vector2 Vector2::operator-(void) const
{
    return Vector2(-x, -y);
}

/******************************************************************************/
/*!
\brief
operator* overload for scalar multiplication

\param  scalar
float to multiply with
\return
Resulting vector
/******************************************************************************/
Vector2 Vector2::operator*(float scalar) const
{
    return Vector2(scalar * x, scalar * y);
}

/******************************************************************************/
/*!
\brief
Return length/magnitude of vector

\return
Length of vector
*/
/******************************************************************************/
float Vector2::Length(void) const
{
    return sqrt(x * x + y * y);
}

/******************************************************************************/
/*!
\brief
Dot product of 2 vectors

\param  rhs
Point to calculate dot product with
\return
float value of dot product
*/
/******************************************************************************/
float Vector2::Dot(const Vector2& rhs)
{
    return x * rhs.x + y * rhs.y;
}

/******************************************************************************/
/*!
\brief
Return a normalized copy of this vector (magnitude of vector is 1)

\param  rhs
Point to calculate distance from
\return
Resulting normalized vector
*/
/******************************************************************************/
Vector2 Vector2::Normalized(void)
{
    float d = Length();
    return Vector2(x / d, y / d);
}