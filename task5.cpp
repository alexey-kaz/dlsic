#include <iostream>

class A {
public:
    static void *operator new(size_t size) {
        std::cout << "operator new!" << std::endl;
        return ::operator new(size);
    }

    static void operator delete(void *p, size_t size) {
        std::cout << "operator delete!" << std::endl;
        return ::operator delete(p);
    }
};

template<typename T1, typename T2>
struct Allocator {
    Allocator() = default;

    using value_type = T1;

    template<typename T3, typename T4>
    constexpr explicit Allocator(const Allocator<T3, T4> &) {};

    T1 *allocate(std::size_t n) {
        std::cout << "Allocate" << std::endl;
        return static_cast<T1 *>(T2::operator new(n * sizeof(T1)));
    }

    void deallocate(T1 *p, std::size_t n) {
        std::cout << "Deallocate" << std::endl;
        T2::operator delete(p, n * sizeof(T1));
    }
};

int main() {
    auto sp = std::allocate_shared<A>(Allocator<A, A>());
}