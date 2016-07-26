#include "Block.h"


void Block::setSize(float w, float h)
{
	_width  = w;
	_height = h;
}

bool Block::getVisible()
{
	return _visible;
}

void Block::unVisible()
{
	_visible = false;
}

Block::Block() { _visible = true; }
Block::~Block() {}
