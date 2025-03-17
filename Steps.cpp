#include <fstream>
#include <iostream>
#include "Steps.h"

Steps Steps::loadSteps(const std::string& filename) {
	Steps steps;
	std::ifstream steps_file(filename);
	steps_file >> steps.randomSeed;
	steps_file >> steps.isColored;
	size_t size;
	steps_file >> size;
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		steps.addStep(iteration, step);
	}
	steps_file.close();
	return steps;
}

void Steps::saveSteps(const std::string& filename) const {
	std::ofstream steps_file(filename, std::ios::out | std::ios::trunc);
	if (!steps_file.is_open()) {
		std::cerr << "Failed to open or create steps file: " << filename << std::endl;
		return;
	}

	steps_file << randomSeed << '\n' << isColored << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
}
bool Steps::isEmpty() const {
	return steps.empty();
}
