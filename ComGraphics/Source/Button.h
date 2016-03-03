/******************************************************************************/
/*!
\file	Button.h
\author 3000 Lines Studio
\brief
Button header file
*/
/******************************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

/******************************************************************************/
/*!
Struct Button:
\brief	Stores button information
*/
/******************************************************************************/
struct Button
{
    float minX, maxX, minY, maxY;       ///< Minimum and Maximum X and Y for hit-box

    void Set(float factorMinX, float factorMaxX, float factorMinY, float factorMaxY);
};

#endif