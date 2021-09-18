// Random Access Container
#ifndef JB_PTR_VISUALIZER_TESTING_RACONT_H
#define JB_PTR_VISUALIZER_TESTING_RACONT_H

#include <random>
#include <chrono>
#include <algorithm>

namespace racont {

    template<typename T, typename Gen = std::mt19937, typename Alloc = std::allocator<T>>
    class racont {

        struct Handler {

            std::size_t capacity;
            T *data;

            explicit Handler(std::size_t new_capacity = 0) : capacity(ceil_pow_2(new_capacity)) {
                if (capacity > 0) {
                    data = Alloc().allocate(capacity);
                }
            }

            Handler(Handler &&) = delete;

            Handler(Handler &) = delete;

            Handler &operator=(Handler &) = delete;

            Handler &operator=(Handler &&) = delete;

            T &operator[](std::size_t i) & noexcept {
                return data[i];
            }

            T &&operator[](std::size_t i) && noexcept {
                return std::move(data[i]);
            }

            void swap(Handler &rhs) noexcept {
                using std::swap;
                swap(this->capacity, rhs.capacity);
                swap(this->data, rhs.data);
            }

            ~Handler() {
                if (capacity > 0) {
                    Alloc().deallocate(data, capacity);
                }
            }

            static std::size_t ceil_pow_2(std::size_t n) noexcept {
                // https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
                n--;
                for (int i = 0; i <= 5; i++) {
                    n |= n >> (1 << i);
                }
                n++;
                return n;
            }
        };

        std::size_t _size{0};
        Gen gen;
        Handler handler;

    public:

        explicit racont(unsigned long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count()) :
                gen(seed), handler() {
        }

        [[nodiscard]] std::size_t size() const noexcept {
            return _size;
        }

        [[nodiscard]] std::size_t capacity() const noexcept {
            return handler.capacity;
        }

        T &operator[](std::size_t i) & noexcept {
            return handler[i];
        }

        T &&operator[](std::size_t i) && noexcept {
            return std::move(handler[i]);
        }

        void insert(std::size_t pos, const T &value) &{
            if (_size >= handler.capacity) {
                Handler new_handler(_size + 1);
                if (pos != 0) {
                    std::uninitialized_move(handler.data, handler.data + pos, new_handler.data);
                }
                std::uninitialized_move(handler.data + pos, handler.data + _size,
                                        new_handler.data + pos + 1);
                Alloc().construct(new_handler.data + pos, value);
                std::destroy(handler.data, handler.data + _size);
                _size++;
                handler.swap(new_handler);
            } else {
                std::copy_backward(handler.data + pos, handler.data + _size, handler.data + _size + 1);
                handler.data[pos] = value;
                _size++;
            }
        }


    };

}

#endif //JB_PTR_VISUALIZER_TESTING_RACONT_H
