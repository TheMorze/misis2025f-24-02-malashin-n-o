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

