#pragma once
#include "Ghost.h"
class SimpleGhost : public Ghost
{
public:
	SimpleGhost(int startX, int startY, Board& board, Mario& mario, bool colored, long seed)
		: Ghost(startX, startY, false, board, mario, colored, seed) {
	}

	virtual void draw() const override;
	virtual void move() override;
};
