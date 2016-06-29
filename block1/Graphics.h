#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>

class Bitmap;

class Graphics
{
public:
    Graphics(HDC hdc);
    Graphics(Bitmap& bitmap);
    Graphics(Graphics& g);
    virtual void init();
    virtual ~Graphics();
    virtual void dispose();

    virtual void setFont(HFONT font);
    virtual void setTextColor(int r, int g, int b);
    virtual void setTextAlign(UINT align);

    virtual void setPen(HPEN pen);

    virtual void drawString(int x, int y, const char* str);
    virtual void drawEllipse(int left, int top, int right, int bottom);

    virtual void copy(Graphics& g, int x, int y, int w, int h, int sx, int sy);
    virtual void copy(Graphics& g, int x, int y, int w, int h, int sx, int sy, int sw, int sh);

    virtual void clear(int width, int height);

private:
    HDC     _hdc;
    Bitmap* _bmp;
    HBITMAP _hBmp;
    HFONT   _font;
    HPEN    _pen;
    HBRUSH  _brush;

};

#endif // _GRAPHICS_H_
