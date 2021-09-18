// Random Access Container
#ifndef JB_PTR_VISUALIZER_TESTING_RACONT_H
#define JB_PTR_VISUALIZER_TESTING_RACONT_H

#include <random>
#include <chrono>
#include <algorithm>
#include <memory>

namespace NRacont {

    template<typename T, typename Gen = std::mt19937, typename Alloc = std::allocator<T>>
    class TRacont {
        class TNode {
            T value;
            std::unique_ptr<TNode> left;
            std::unique_ptr<TNode> right;
        };

        std::unique_ptr<TNode> root;
        

    };

}

#endif //JB_PTR_VISUALIZER_TESTING_RACONT_H
