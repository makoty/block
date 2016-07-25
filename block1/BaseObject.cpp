#include "BaseObject.h"

BaseObject::BaseObject()
{
}


BaseObject::~BaseObject()
{
}

void BaseObject::setPos(short x, short y) {
    _posX = x;
    _posY = y;
}

short BaseObject::getVX()   { return _vX;   }
short BaseObject::getVY()   { return _vY;   }
short BaseObject::getPosX() { return _posX; }
short BaseObject::getPosY() { return _posY; }
short BaseObject::getWidth()  { return _width; }
short BaseObject::getHeight() { return _height;}