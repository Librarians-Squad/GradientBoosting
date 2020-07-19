#ifndef GRADIENTBOOSTING_UTILITY_HPP
#define GRADIENTBOOSTING_UTILITY_HPP

#include <utility>

// TODO(Mocurin): O(1)/O(N^m) space/time complexity
template <typename RandIt1, typename RandIt2>
void reorder(RandIt1 first1, RandIt1 last1, RandIt2 first2) {
    size_t i = 0;
    for (size_t j = 0; j < last1 - first1 - 1; ++j) {
        i = *(first2 + j);
        while (i < j) i = *(first2 + i);
        std::iter_swap(first1 + i, first1 + j);
    }
}

#endif //GRADIENTBOOSTING_UTILITY_HPP
