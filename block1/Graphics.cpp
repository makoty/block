#include "Graphics.h"
#include "Bitmap.h"
#include <string.h>

Graphics::Graphics(HDC hdc)
    : _hdc(hdc), _bmp(nullptr), _hBmp(NULL)
    , _font(NULL), _pen(NULL), _brush(NULL)
{
    init();
}

Graphics::Graphics(Bitmap& bitmap)
{
    _hdc = CreateCompatibleDC(bitmap.getRawDC());
    if (!_hdc) throw;
    _bmp  = &bitmap;
    _hBmp = (HBITMAP)SelectObject(_hdc, bitmap.getRawBitmap());
    init();
}

Graphics::Graphics(Graphics& g)
    : _hdc  (g._hdc)
    , _bmp  (g._bmp)
    , _hBmp (g._hBmp)
    , _font (g._font)
    , _pen  (g._pen)
    , _brush(g._brush)
{
}

void Graphics::init()
{
    _brush = (HBRUSH)SelectObject(_hdc, GetStockObject(NULL_BRUSH));
    SetBkMode(_hdc, TRANSPARENT);
    SetStretchBltMode(_hdc, HALFTONE);
}

Graphics::~Graphics()
{
    dispose();
}

void Graphics::dispose()
{
    if (_hBmp)
    {
        SelectObject(_hdc, _hBmp);
        DeleteDC(_hdc);
    }
    _hdc  = NULL;
    _bmp  = nullptr;
    _hBmp = NULL;
}

void Graphics::setFont(HFONT font)
{
    if (font)
    {
        HFONT tmp = (HFONT)SelectObject(_hdc, font);
        if (!_font) _font = tmp;
    }
    else if (_font)
    {
        SelectObject(_hdc, _font);
        _font = NULL;
    }
}

void Graphics::setTextColor(int r, int g, int b)
{
    SetTextColor(_hdc, RGB(r, g, b));
}

void Graphics::setTextAlign(UINT align)
{
    SetTextAlign(_hdc, align);
}

void Graphics::setPen(HPEN pen)
{
    if (pen)
    {
        HPEN tmp = (HPEN)SelectObject(_hdc, pen);
        if (!_pen) _pen = tmp;
    }
    else if (_pen)
    {
        SelectObject(_hdc, _pen);
        _pen = NULL;
    }
}

void Graphics::drawString(int x, int y, const char* str)
{
    TextOutA(_hdc, x, y, str, strlen(str));
}

void Graphics::drawEllipse(int left, int top, int right, int bottom)
{
    Ellipse(_hdc, left, top, right, bottom);
}

void Graphics::copy(Graphics& g, int x, int y, int w, int h, int sx, int sy)
{
    BitBlt(_hdc, x, y, w, h, g._hdc, 0, 0, SRCCOPY);
}

void Graphics::copy(Graphics& g, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
{
    StretchBlt(_hdc, x, y, w, h, g._hdc, 0, 0, sw, sh, SRCCOPY);
}

void Graphics::clear(int width, int height)
{
    PatBlt(_hdc, 0, 0, width, height, BLACKNESS);
}
