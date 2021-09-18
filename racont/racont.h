// Random Access Container
#ifndef JB_PTR_VISUALIZER_TESTING_RACONT_H
#define JB_PTR_VISUALIZER_TESTING_RACONT_H

#include <random>
#include <chrono>
#include <algorithm>
#include <memory>

namespace NRacont {

    template<typename T>
    struct ITreap {
        struct TNode {
            T value;
            int priority;
            std::size_t size;
            std::shared_ptr<TNode> left;
            std::shared_ptr<TNode> right;

            TNode(const T &value_other, int priority_other) : value(value_other), priority(priority_other), size(1),
                                                              left(nullptr), right(nullptr) {
            }

            static std::size_t get_size(const std::shared_ptr<TNode> &node) {
                return node ? node->size : 0;
            }

            static void update(const std::shared_ptr<TNode> &node) {
                if (node) {
                    node->size = 1 + get_size(node->left) + get_size(node->right);
                }
            }
        };

        std::shared_ptr<ITreap::TNode> root;

        std::pair<std::shared_ptr<TNode>, std::shared_ptr<TNode>>
        split(std::shared_ptr<TNode> vertex, std::size_t pivot) {
            if (!vertex) {
                return {nullptr, nullptr};
            }
            if (vertex->value >= pivot) {
                auto divided = split(vertex->left, pivot);
                root->left = divided.second;
                TNode::update(root);
                return {divided.first, root};
            } else {
                auto divided = split(vertex->right, pivot);
                root->right = divided.first;
                TNode::update(root);
                return {root, divided.second};
            }
        }

        std::shared_ptr<TNode> merge(std::shared_ptr<TNode> left, std::shared_ptr<TNode> right) {
            if (!left) {
                return right;
            }
            if (!right) {
                return left;
            }
            if (left->priority > right->priority) {
                left->right = merge(left->right, right);
                TNode::update(left);
                return left;
            } else {
                right->left = merge(left, right->left);
                TNode::update(right);
                return right;
            }
        }
    };

    template<typename T, typename Gen = std::mt19937, typename Alloc = std::allocator<T>>
    class TRacont : ITreap<T> {
        using typename ITreap<T>::TNode;

        Gen gen;

    public:

        TRacont(int seed = 0) : ITreap<T>(), gen(seed) {
        }

        [[nodiscard]] std::size_t size() const {
            return TNode::get_size(this->root);
        }

        void insert(const T &value) &{
            auto new_node = std::allocate_shared<TNode>(Alloc(), value, gen());
            this->root = this->merge(this->root, new_node);
        }
    };

}

#endif //JB_PTR_VISUALIZER_TESTING_RACONT_H
