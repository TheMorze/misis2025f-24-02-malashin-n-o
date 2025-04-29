#pragma once
#include <vector>
#include <cstdint>
#include <chrono>

struct Color {
    uint8_t r, g, b;
};

using Image = std::vector<std::vector<Color>>;

// Простое среднее размытие k×k
Image sequentialBlur(const Image& input, int k);

// Параллельное размытие с потоками
Image parallelBlurThreads(const Image& input, int k, int numThreads);

// Измерение времени выполнения
template<typename F, typename... Args>
auto measure(F func, Args&&... args) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::make_pair(result,
        std::chrono::duration<double, std::milli>(t2 - t1).count());
}