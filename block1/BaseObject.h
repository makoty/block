#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();

	void  setPos(short x, short y);

    short getVX();
    short getVY();
	short getPosX();
	short getPosY();
    short getWidth();
    short getHeight();

protected:
    short _vX; // X•ûŒü‘¬“x
    short _vY; // Y•ûŒü‘¬“x
	short _posX;
	short _posY;
    short _width;
    short _height;
};

#endif