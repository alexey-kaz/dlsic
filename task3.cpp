#include <iostream>
#include <vector>


int main() {
    std::vector<bool> v(false);
    for (auto &x: v) {
        x = true;
    }
    for (auto &&x: v) {
        x = true;
    }
}

/*
 * Ошибка компиляции в применении for (auto& x: v) {...}
 * Non-const lvalue reference to type '__bit_reference<...>' cannot bind to a temporary of type '__bit_reference<...>'
 *
 * Проблема первого случая в том, что нельзя "связать" временный объект с некостантной ссылкой lvalue
 *
 * && x решает эту проблему, так как он принимает любой инициализатор, независимо от того,
 * является ли он выражением lvalue или rvalue
 */
