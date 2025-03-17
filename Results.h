#pragma once

#include <list>
#include <string>
#include <utility>
#include <tuple>
class Results {
public:
    enum ResultValue { hit, finished, score };
private:
    std::list<std::tuple<size_t, ResultValue, size_t>> results;
public:
    void ClearResults() {
        results.clear();
    }
    bool hasOnlyOneResultLeft() const {
        return results.size() == 1;
    }
    static Results loadResults(const std::string& filename);
    void saveResults(const std::string& filename) const;
    void addResult(size_t iteration, ResultValue result, size_t score) {
        results.push_back({ iteration, result,score });
    }
    std::tuple<size_t, ResultValue, size_t> popResult() {
        if (results.empty()) return { 0, Results::score ,0 };
        auto result = results.front();
        results.pop_front();
        return result;
    }
    std::tuple<size_t, ResultValue, size_t> peekLastResult() const {
        if (results.empty()) {
            return { 0, ResultValue::score, 0 }; // Default values
        }
        auto result = results.front();
        return result;
    }
    size_t getNextResult() const;
    bool isFinishedBy(size_t iteration) const {
        return results.empty() || std::get<0>(results.back()) < iteration;
    }

    bool isEmpty() const;
};