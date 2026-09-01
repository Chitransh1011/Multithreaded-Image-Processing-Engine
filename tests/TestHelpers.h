#pragma once

#include <exception>
#include <iostream>
#include <string>

namespace test {

inline int failures = 0;

inline void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        ++failures;
    }
}

template <typename Function>
void expectThrows(Function&& function, const std::string& message) {
    try {
        function();
        expect(false, message);
    } catch (const std::exception&) {
    }
}

inline int finish() {
    if (failures == 0) {
        std::cout << "All checks passed.\n";
    }
    return failures == 0 ? 0 : 1;
}

}  // namespace test
