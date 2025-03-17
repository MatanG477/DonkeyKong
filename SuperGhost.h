#pragma once

#include "Ghost.h"
class SuperGhost : public Ghost {
public:
	SuperGhost(int startX, int startY, Board& board, Mario& mario, bool colored, long seed)
		: Ghost(startX, startY, true, board, mario, colored, seed) {
	}
	virtual void draw() const override;
	virtual void move() override;
	bool isLadder() const;
	bool isCeiling() const;
	void SetIsClimb(bool isClimb) { this->isClimb = isClimb; };
	bool isClimb = false;
};