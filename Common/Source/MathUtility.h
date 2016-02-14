/******************************************************************************/
/*!
\file	MathUtility.h
\author Foo Jing Ting
\par	email: 152856H\@nyp.edu.sg
\brief
Functions of math formulae
*/
/******************************************************************************/

#ifndef MATH_UTILITY_H
#define MATH_UTILITY_H

int factorial( int n );     //return the factorial of n
int nCr( int n, int r );    //return the number of combinations, given n and r
int nPr( int n, int r );    //return the number of permutations, given n and r

int nTermAP( int a, int d, int n );      //return the n term of an arithmetic progression, given the first term and the difference
int summationAP( int a, int d, int n );       //return the summation of the first n terms of an arithmetic progression, given the first term and the difference
int nTermGP( int a, int r, int n );      //return the n term of a geometric progression, given the first term and the difference
int summationGP( int a, int r, int n );       //return the summation of the first n terms of a geometric progression, given the first term and the difference

#endif