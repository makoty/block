#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "BaseObject.h"

class Block : public BaseObject
{
public:
	void setSize(float w, float h);
	bool getVisible();
	void unVisible();
	Block();
	~Block();
private:
	bool _visible;
};

#endif