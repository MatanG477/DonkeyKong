#include <limits>
#include <fstream>
#include <tuple>
#include <iostream>
#include "Results.h"

Results Results::loadResults(const std::string& filename) {
    std::ifstream results_file(filename);
    Results results;
    size_t size;
    results_file >> size;
    while (!results_file.eof() && size-- != 0) {
        size_t iteration;
        int result;
        size_t score;
        results_file >> iteration >> result >> score;
        results.addResult(iteration, static_cast<ResultValue>(result), score);
    }
    return results;
}

void Results::saveResults(const std::string& filename) const {
    std::ofstream results_file(filename, std::ios::out | std::ios::trunc);
    if (!results_file.is_open()) {
        std::cerr << "Failed to open or create results file: " << filename << std::endl;
        return;
    }

    results_file << results.size();
    for (const auto& result : results) {
        results_file << '\n' << std::get<0>(result) << ' ' << std::get<1>(result) << ' ' << std::get<2>(result);
    }
    results_file.close();
}



size_t Results::getNextResult() const {
    return std::get<0>(results.front());
}

bool Results::isEmpty() const {
    return results.empty();
}