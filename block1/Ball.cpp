#include "Const.h"
#include "Ball.h"


Ball::Ball()
{
    _radius = 7;    // ���a
    _vecX   = 0;    // X�������x
    _vecY   = 5;    // Y�������x
}

Ball::~Ball()
{
}

short Ball::getRadius() { return _radius; }

void Ball::move() {
    _posX += _vecX;
    _posY += _vecY;
}