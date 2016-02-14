/******************************************************************************/
/*!
\file	MathUtility.cpp
\author Foo Jing Ting
\par	email: 152856H\@nyp.edu.sg
\brief
Functions of math formulae
*/
/******************************************************************************/

#include "MathUtility.h"

/******************************************************************************/
/*!
\brief
Factorial function: multiply together all integers from 1 to n

\param  n
Number to calculate factorial

\return
Factorial of n
Error check: return -1 if n < 0 or n > 12
*/
/******************************************************************************/
int factorial(int n)
{
    //error check - factorials of negative numbers are undefined
    if (n < 0) {
        return -1;
    }

    //error check - factorials of numbers 13 or above is too large to be represented by int
    if (n > 12) {
        return -1;
    }

    if (n == 0) {
        return 1;
    }
    return factorial(n-1) * n;
}

/******************************************************************************/
/*!
\brief
Number of ways a subset of r items can be chosen from a set of n items

\param  n
Set size
\param  r
Subset size

\return
Number of combinations
Error check: return -1 if n < 0 or r < 0; or if r > n; or if n > 12 or r > 12
*/
/******************************************************************************/
int nCr(int n, int r)
{
    //error check - there cannot be sets of negative n and r number of items
    if (n < 0 || r < 0) {
        return -1;
    }

    //error check - there cannot be a larger subset size than the set size
    if (r > n) {
        return -1;
    }
    
    //error check - as underlying formula or nCr uses factorial,
    //factorials of numbers 13 or above is too large to be represented by int
    if (n > 12 || r > 12) {
        return -1;
    }

    //formula: nCr = n! / ( r! (n-r)! )
    return factorial(n) / (factorial(r) * factorial(n - r));
}

/******************************************************************************/
/*!
\brief
Number of ways a subset of r items can be chosen out of a set of n items.
Different arrangements of the same items are counted

\param  n
Set size
\param  r
Subset size

\return
Number of permutations
Error check: return -1 if n < 0 or r < 0; or if r > n; or if n > 12 or r > 12
*/
/******************************************************************************/
int nPr(int n, int r)
{
    //error check - there cannot be sets of negative n and r number of items
    if (n < 0 || r < 0) {
        return -1;
    }

    //error check - there cannot be a larger subset size than the set size
    if (r > n) {
        return -1;
    }

    //error check - as underlying formula or nCr uses factorial,
    //factorials of numbers 13 or above is too large to be represented by int
    if (n > 12 || r > 12) {
        return -1;
    }

    //formula: nPr = n! / (n - r)!
    return factorial(n) / factorial(n - r);
}

/******************************************************************************/
/*!
\brief
Return the n term of an arithmetic progression

\param  a
First term
\param  d
Constant difference between each term
\param  n
Term to be found

\return
n term of an arithmetic progression
Error check: return -1 if n < 0
*/
/******************************************************************************/
int nTermAP(int a, int d, int n)
{
    //error check - there cannot be a term of negative or zero position
    if (n <= 0) {
        return -1;
    }
    
    return a + d * (n - 1);
}

/******************************************************************************/
/*!
\brief
Return the summation of the first n terms of an arithmetic progression

\param  a
First term
\param  d
Constant difference between each term
\param  n
Term to be found

\return
sum of first n terms of an arithmetic progression
Error check: return -1 if n < 0
*/
/******************************************************************************/
int summationAP(int a, int d, int n)
{
    //error check - there cannot be a term of negative or zero position
    if (n <= 0) {
        return -1;
    }

    if (n == 1) {
        return a;
    }
    return summationAP(a, d, (n - 1)) + a + d * (n - 1);
}

/******************************************************************************/
/*!
\brief
Return the n term of a geometric progression

\param  a
First term
\param  d
Common ratio
\param  n
Term to be found

\return
n term of a geometric progression
Error check: return -1 if n < 0
*/
/******************************************************************************/
int nTermGP(int a, int r, int n)
{
    //error check - there cannot be a term of negative or zero position
    if (n <= 0) {
        return -1;
    }

    //formula: n-th term = (first term) + r ^ (n - 1)
    /*for (int i = 0; i < (n - 1); i++) {
        r *= r;
    }
    return a * r;*/

    if (n == 1) {
        return a;
    }
    return nTermGP(a, r, n - 1) * r;
}

/******************************************************************************/
/*!
\brief
Return the summation of the first n terms of a geometric progression

\param  a
First term
\param  d
Common ratio
\param  n
Term to be found

\return
sum of first n terms of a geometric progression
Error check: return -1 if n < 0
*/
/******************************************************************************/
int summationGP(int a, int r, int n)
{
    //error check - there cannot be a term of negative or zero position
    if (n <= 0) {
        return -1;
    }

    if (n == 1) {
        return a;
    }
    return summationGP(a, r, (n - 1)) * r + a;
}