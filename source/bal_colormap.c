#include "bal_colormap.h"
#include <malloc.h>
#include <math.h>
#include "bal_point.h"
#include <stdio.h>

Int RoundInt(Float number)
{
    return (Int)(number + .5f);
}

PColorMap ColorMapCreate(const Int width, const Int height)
{
    PColorMap _map = malloc(sizeof(ColorMap));
    _map->width = width;
    _map->height = height;
    _map->linearSize = width * height;
    PColor _clr = malloc(_map->linearSize * sizeof(Color));
    for (Int x = 0; x < _map->linearSize; x++)
    {
        _clr[x] = ColorCreate(255, 255, 255);
    }
    _map->colors = _clr;
    _map->ratio = (Float)width / (Float)height;
    return _map;
}

void ColorMapDestroy(const PColorMap colorMap)
{
    free(colorMap->colors);
    free(colorMap);
}

void ColorMapResize(PColorMap colorMap, const Int newWidth, const Int newHeight)
{
    Int _nW;
    Int _nH;
    if (newWidth < colorMap->width)
        _nW = newWidth;
    else
        _nW = colorMap->width;
    if (newHeight < colorMap->height)
        _nH = newHeight;
    else
        _nH = colorMap->height;
    Int newLinearSize = newWidth * newHeight;
    PColor _clr = malloc(newLinearSize * sizeof(Color));
    Color __color = {0};
    PColor __colorPtr = &__color;
    for (Int y = 0; y < _nH; y++)
    {
        for (Int x = 0; x < _nW; x++)
        {
            if (ColorMapGetPixelA(colorMap, x, y, __colorPtr))
            {
                _clr[y * newWidth + x] = __color;
            }
        }
    }
    PColor __ = colorMap->colors;
    colorMap->width = newWidth;
    colorMap->height = newHeight;
    colorMap->colors = _clr;
    colorMap->ratio = (Float)newWidth / (Float)newHeight;
    colorMap->linearSize = newLinearSize;
    free(__);
}

Int ColorMapSetPixel(const register PColorMap colorMap, const register Point2D position, const register Color color)
{
    if (colorMap == 0)
        return 0;
    if ((position.x | position.y) >= 0 && position.y < colorMap->height && position.x < colorMap->width)
    {
        ColorMapPixel(colorMap, position.x, position.y) = color;
        return 1;
    }
    return 0;
}

Int ColorMapSetPixelA(const register PColorMap colorMap, const register Int x, const register Int y, const register Color color)
{
    if (colorMap == 0)
        return 0;
    if ((x | y) >= 0 && y < colorMap->height && x < colorMap->width)
    {
        ColorMapPixel(colorMap, x, y) = color;
        return 1;
    }
    return 0;
}

Int ColorMapGetPixel(const PColorMap colorMap, const Point2D position, PColor colorOut)
{
    if (colorMap == 0)
        return 0;
    if ((position.x | position.y) >= 0 && position.y * colorMap->width + position.x < colorMap->linearSize)
    { 
        *colorOut = ColorMapPixel(colorMap, position.x, position.y);
        return 1;
    }
    return 0;
}

Int ColorMapGetPixelA(const PColorMap colorMap, const Int x, const Int y, PColor colorOut)
{
    if (colorMap == 0)
        return 0;
    if ((x | y) >= 0 && y * colorMap->width + x < colorMap->linearSize)
    { 
        *colorOut = ColorMapPixel(colorMap, x, y);
        return 1;
    }
    return 0;
}

void ColorMapDrawLine(const PColorMap colorMap, Point2D start, Point2D end, const Color color)
{
    Int _a = end.x - start.x;
    Int _b = end.y - start.y;
    const Int _w = colorMap->width;
    const Int _h = colorMap->height;
    Int sX;
    Int eX;
    const char _check = abs(_b) > abs(_a);
    if (_check)
    {
        register Int _s = start.x;
        start.x = start.y;
        start.y = _s;
        _s = end.x;
        end.x = end.y;
        end.y = _s;
        _s = _a;
        _a = _b;
        _b = _s;
    }
    if (start.x < end.x)
    {
        sX = start.x;
        eX = end.x;
    }
    else
    {
        sX = end.x;
        eX = start.x;
    }
    const register Float n = (Float)_b / (Float)_a;
    const register Float m = start.y - start.x * n;
    if (_check)
    {
        for (; sX < eX; sX++)
        {
            ColorMapSetPixelA(colorMap, (Int)(sX * n + m + .5f), sX, color);
        }
        return;
    }
    for (; sX < eX; sX++)
    {
        ColorMapSetPixelA(colorMap, sX, (Int)(sX * n + m + .5f), color);
    }
}

void ColorMapDrawLineA(const PColorMap colorMap, Int startX, Int startY, Int endX, Int endY, const Color color)
{
    ColorMapDrawLine(colorMap, Point2DCreate(startX, startY), Point2DCreate(endX, endY), color);
}

void ColorMapClear(const PColorMap colorMap)
{
    for (Int x = 0; x < colorMap->width; x++)
    {
        for (Int y = 0; y < colorMap->height; y++)
        {
            ColorMapSetPixelA(colorMap, x, y, ColorCreate(255, 255, 255));
        }
    }
}