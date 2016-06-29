#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <windows.h>

#include "Graphics.h"

class Bitmap
{
public:
    Bitmap(HDC hdc, int width, int height);
    Bitmap(HBITMAP hBitmap);
    virtual ~Bitmap();
    virtual void dispose();

    virtual HBITMAP getRawBitmap() const;
    virtual HDC     getRawDC() const;

private:
    HBITMAP _hBitmap;
    HDC     _hdc;
    int _width;
    int _height;

};

#endif // _BITMAP_H_
