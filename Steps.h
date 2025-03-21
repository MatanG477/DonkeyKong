#pragma once

#include <string>
#include <list>

class Steps {
	long randomSeed = 0;
	bool isColored = false;
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step
public:
	static Steps loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename) const;
	long getRandomSeed() const {
		return randomSeed;
	}
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}
	void setIsColored(bool isColored)
	{
		this->isColored = isColored;
	}
	bool getIsColored() const
	{
		return isColored;
	}
	void addStep(size_t iteration, char step) {
		steps.push_back({ iteration, step });
	}
	bool isNextStepOnIteration(size_t iteration) const {
		return !steps.empty() && steps.front().first == iteration;
	}
	char popStep() {
		char step = steps.front().second;
		steps.pop_front();
		return step;
	}
	bool isEmpty() const;
	void ClearSteps() {
		steps.clear();
	}
};