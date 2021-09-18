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
            std::size_t size;
            std::shared_ptr<TNode> left;
            std::shared_ptr<TNode> right;

            TNode(const T &value_other) : value(value_other), size(1),
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


        std::pair<std::shared_ptr<TNode>, std::shared_ptr<TNode>>
        split(std::shared_ptr<TNode> node, const T &pivot) {
            if (!node) {
                return {nullptr, nullptr};
            }
            if (node->value >= pivot) {
                auto divided = split(node->left, pivot);
                node->left = divided.second;
                TNode::update(node);
                return {divided.first, node};
            } else {
                auto divided = split(node->right, pivot);
                node->right = divided.first;
                TNode::update(node);
                return {node, divided.second};
            }
        }

        std::mt19937 _gen;

        std::shared_ptr<TNode> merge(std::shared_ptr<TNode> left, std::shared_ptr<TNode> right) {
            if (!left) {
                return right;
            }
            if (!right) {
                return left;
            }
            auto random = static_cast<std::size_t>(_gen()) % (TNode::get_size(left) + TNode::get_size(right));
            if (random < TNode::get_size(left)) {
                left->right = merge(left->right, right);
                TNode::update(left);
                return left;
            } else {
                right->left = merge(left, right->left);
                TNode::update(right);
                return right;
            }
        }

        ITreap() : _gen(std::chrono::high_resolution_clock::now().time_since_epoch().count()) {}
    };

    template<typename T, typename Gen = std::mt19937, typename Alloc = std::allocator<T>>
    class TRacont : ITreap<T> {
        using typename ITreap<T>::TNode;
        using ITreap<T>::split;
        using ITreap<T>::merge;

        Gen gen;
        std::shared_ptr<TNode> root;

        T &traverse(std::shared_ptr<TNode> node, std::size_t pos) {
            if (TNode::get_size(node->left) < pos) {
                return traverse(node->right, pos - TNode::get_size(node->left) - 1);
            } else if (TNode::get_size(node->left) == pos) {
                return node->value;
            } else {
                return traverse(node->left, pos);
            }
        }

    public:

        TRacont(int seed = 0) : ITreap<T>(), gen(seed) {
        }

        [[nodiscard]] std::size_t size() const {
            return TNode::get_size(this->root);
        }

        void insert(const T &value) &{
            auto new_node = std::allocate_shared<TNode>(Alloc(), value);
            auto divided = split(root, value);
            divided.first = merge(divided.first, new_node);
            root = merge(divided.first, divided.second);
        }

        void erase(const T &value) &{
            auto split1 = split(root, value);
            auto split2 = split(split1.second, value + 1);
            split2.first = merge(split2.first->left, split2.first->right);
            split1.second = merge(split2.first, split2.second);
            root = merge(split1.first, split1.second);
        }

        const T &operator()() {
            auto pos = static_cast<std::size_t>(gen() % TNode::get_size(root));
            return traverse(root, pos);
        }
    };

}

#endif //JB_PTR_VISUALIZER_TESTING_RACONT_H
