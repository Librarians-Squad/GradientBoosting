#ifndef GRADIENTBOOSTING_UTILITY_HPP
#define GRADIENTBOOSTING_UTILITY_HPP

#include <vector>

template <typename RandIt>
class Range {
public:
    Range(RandIt begin, RandIt end) :
            start(begin), stop(end) {}

    RandIt begin() { return start; }
    RandIt end() { return stop; }

    size_t size() const {
        return std::distance(start, stop);
    }

private:
    RandIt start;
    RandIt stop;
};

template <typename T>
void reorder(const Range<std::vector<T>>& source,
        const Range<std::vector<size_t>>& indices) {
    // Reorder array according to indices
}

#endif //GRADIENTBOOSTING_UTILITY_HPP
