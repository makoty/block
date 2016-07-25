#ifndef _BALL_H_
#define _BALL_H_

#include "BaseObject.h"

class Ball : public BaseObject
{
public:
    
    short Ball::getRadius();
    void  Ball::move();

    Ball();
    ~Ball();
private:
    const static short TYPE_WHITE  = 0;
    const static short TYPE_BLUE   = 1;
    const static short TYPE_YELLOW = 2;
    const static short TYPE_GREEN  = 3;
    const static short TYPE_RED    = 4;

    short _radius;
    short _vecX;
    short _vecY;
    short _colorType; // �{�[����� ���A�A���A�΁A��
    short _color[5];


};

#endif