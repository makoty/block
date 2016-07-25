#include "Const.h"
#include "Bar.h"

Bar::Bar() {
    _vX     = 5;
    _vY     = 5;
    _width  = 120;
    _height = 8;
}


void Bar::addPos(short x, short y) {
    short tmpX = _posX + x;
    if (x != 0 && (tmpX >= 0) && tmpX < (DWIDTH - 1)) {
        _posX += x;
    }

    short tmpY = _posY + y;
    if (y != 0 && (tmpY > 0) && tmpY < (DHEIGHT - 1)) {
        _posY += y;
    }
}

Bar::~Bar() {}