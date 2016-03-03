/******************************************************************************/
/*!
\file	Utility.h
\author 3000 Lines Studio
\brief
Utility header file
*/
/******************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include "Vertex.h"
#include "Mtx44.h"

Position operator*(const Mtx44& lhs, const Position& rhs);

#endif