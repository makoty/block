#include "Bitmap.h"

Bitmap::Bitmap(HDC hdc, int width, int height)
    : _hdc(hdc)
    , _width(width), _height(height)
{
    _hBitmap = CreateCompatibleBitmap(hdc, width, height);
    if (!_hBitmap) throw;
}

Bitmap::Bitmap(HBITMAP hBitmap)
    : _hBitmap(hBitmap)
    , _hdc(NULL)
{
    BITMAP bm;
    GetObject(hBitmap , sizeof(BITMAP) , &bm);
    _width  = bm.bmWidth;
    _height = bm.bmHeight;
}

Bitmap::~Bitmap()
{
    dispose();
}

void Bitmap::dispose()
{
    if (_hdc) DeleteObject(_hBitmap);
    _hBitmap = NULL;
    _hdc = NULL;
}

HBITMAP Bitmap::getRawBitmap() const
{
    return _hBitmap;
}

HDC Bitmap::getRawDC() const
{
    return _hdc;
}
