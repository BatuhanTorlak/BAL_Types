#ifndef BAL_COLORMAP_H
#define BAL_COLORMAP_H

#include "bal_color.h"
#include "bal_point.h"

#define ColorMapPixel(map, x, y) ((map)->colors[(y) * (map)->width + (x)])
#define ColorMapPixelA(map, x) map->colors[x]

/// @brief Two dimensional color storage and graphical structer
typedef struct ColorMap_t
{
    PColor colors;
    Int width;
    Int height;
    Int linearSize;
    Float ratio;
    pvoid reserved;
    pvoid other;
} 
/// @brief Two dimensional color storage and graphical structer
ColorMap, *PColorMap;

/// @brief Color map contructer, must destructed after usage
/// @param width Width of color map
/// @param height Height of color map
/// @return Color map created in heap
PColorMap ColorMapCreate(const Int width, const Int height);

/// @brief Color map destructer
/// @param colorMap The pointer of color map created (must be non 0)
void ColorMapDestroy(const PColorMap colorMap);

void ColorMapResize(PColorMap colorMap, const Int newWidth, const Int newHeight);

int ColorMapSetPixel(const PColorMap colorMap, const Point2D position, const register Color color);

int ColorMapSetPixelA(const register PColorMap colorMap, const register Int x, const register Int y, const register Color color);

int ColorMapGetPixel(const PColorMap colorMap, const Point2D position, PColor colorOut);

int ColorMapGetPixelA(const PColorMap colorMap, const Int x, const Int y, PColor colorOut);

/// @brief Draws direct line on color map from start point to end point with selected color
/// @param colorMap The pointer of color map
/// @param start Start point of line
/// @param end End point of line
/// @param color Color of line
void ColorMapDrawLine(const PColorMap colorMap, Point2D start, Point2D end, const Color color);

void ColorMapDrawLineA(const PColorMap colorMap, Int startX, Int startY, Int endX, Int endY, const Color color);

void ColorMapFill(const PColorMap colorMap, const Point2D position, const Color color);

void ColorMapClear(const PColorMap colorMap);

void ColorMapClearA(const PColorMap colorMap, const Color color);

#endif