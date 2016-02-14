/******************************************************************************/
/*!
\file	Vector2.h
\author Foo Jing Ting
\par	email: 152856H\@nyp.edu.sg
\brief
Struct to define a 2D vector
*/
/******************************************************************************/

#ifndef VECTOR2_H
#define VECTOR2_H

#include "MyMath.h"

/******************************************************************************/
/*!
Class Vector2:
\brief	Defines a 2D vector and its methods
*/
/******************************************************************************/
struct Vector2 {
    float x, y;

    Vector2( float a = 0, float b = 0 );  //default constructor
    Vector2( const Vector2& rhs );        //copy constructor
    void Set( float a, float b );     //set all data

    Vector2 operator+( const Vector2& rhs ) const;  //vector addition
    Vector2 operator-( const Vector2& rhs ) const;  //vector subtraction
    Vector2 operator-( void ) const;        //unary negation
    Vector2 operator*( float scalar ) const;    //scalar multiplication

    float Length( void ) const;     //get magnitude
    float Dot( const Vector2& rhs );    //dot product
    Vector2 Normalized(void);       //return a copy of this vector, normalized
};

#endif