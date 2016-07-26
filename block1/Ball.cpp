#include <windows.h>
#include <random>
#include "Const.h"
#include "Ball.h"


Ball::Ball()
{
    _radius = 7;    // ”¼Œa
    _vX   = 5;    // X•ûŒü‘¬“x
    _vY   = 5;    // Y•ûŒü‘¬“x
}

Ball::~Ball()
{
}

short Ball::getRadius() { return _radius; }

void Ball::changeVecX() { _vX *= -1; }
void Ball::changeVecY() { _vY *= -1; }

void Ball::move() {
	if (_visible) {
		_posX += _vX;
		_posY += _vY;
	}
}

bool Ball::getVisible() { return _visible; }

void Ball::unVisible() {
	_visible = false;
}
void Ball::setVisible() {
	_visible = true;
}

void  Ball::setColor(short type) {
	_colorType = type;
}
void  Ball::setRandomColor() {
	std::random_device rnd;
	_colorType = rnd() % 5;
}

COLORREF Ball::getColor() {
	switch (_colorType) {
		case TYPE_WHITE:
			return RGB(255,255,255);
		case TYPE_BLUE:
			return RGB(0,0,255);
		case TYPE_YELLOW:
			return RGB(255,255,0);
		case TYPE_GREEN:
			return RGB(0,255,0);
		case TYPE_RED:
			return RGB(255,0,0);
		default:
			return RGB(255,255,255);
	}
}

short Ball::getPoint() {
	switch (_colorType) {
		case TYPE_WHITE:
			return 0;
		case TYPE_BLUE:
			return 10;
		case TYPE_YELLOW:
			return 50;
		case TYPE_GREEN:
			return 100;
		case TYPE_RED:
			return 500;
		default:
			return 0;
	}
}

void Ball::setVec(short vx, short vy) {
	_vX = vx;
	_vY = vy;
}