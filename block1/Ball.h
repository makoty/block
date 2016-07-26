#ifndef _BALL_H_
#define _BALL_H_

#include "BaseObject.h"

class Ball : public BaseObject
{
public:
    
    short Ball::getRadius();
    void  Ball::move();
	void  Ball::changeVecX();
	void  Ball::changeVecY();
	bool  Ball::getVisible();
	void  Ball::unVisible();
	void  Ball::setVisible();
	COLORREF Ball::getColor();
	void  Ball::setColor(short type);
	void  Ball::setRandomColor();
	void  Ball::setVec(short vx, short vy);
    short Ball::getPoint();

    Ball();
    ~Ball();
private:
    const static short TYPE_WHITE  = 0;
    const static short TYPE_BLUE   = 1;
    const static short TYPE_YELLOW = 2;
    const static short TYPE_GREEN  = 3;
    const static short TYPE_RED    = 4;

	bool  _visible;
    short _radius;
    short _colorType; // �{�[����� ���A�A���A�΁A��
};

#endif