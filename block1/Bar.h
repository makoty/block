#ifndef _BAR_H_
#define _BAR_H_

#include "Const.h"
#include "BaseObject.h"

class Bar : public BaseObject
{
public:
	static Bar& getInstance() {
        static Bar instance;
        return instance;
    }

	void addPos(short x, short y);

private:
    Bar();
    ~Bar();
};

#endif // _BAR_H_