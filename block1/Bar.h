#ifndef _BAR_H_
#define _BAR_H_

class Bar
{
public:
	Bar();
	Bar(short x, short y);
	void setPos(short x, short y);
	short getPosX();
	short getPosY();
	void  addPos(short x, short y);

	~Bar();
private:
	short _posX;
	short _posY;
};

#endif // _BAR_H_