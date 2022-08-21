#pragma once

#include <cstdint>
namespace sim {
struct Vec2 {
    float x{0};
    float y{0};

    [[nodiscard]] double dist(const Vec2& other) const;

    static Vec2 random_vec(float x_min, float x_max, float y_min, float y_max);
} __attribute__((aligned(8))) __attribute__((__packed__));

struct Vec4U {
    uint32_t x{0};
    uint32_t y{0};
    uint32_t z{0};
    uint32_t w{0};

    static Vec4U random_vec();
} __attribute__((aligned(16))) __attribute__((__packed__));

struct Rgba {
    float r{0};
    float g{0};
    float b{0};
    float a{0};

    static Rgba random_color();
} __attribute__((aligned(16))) __attribute__((__packed__));
}  // namespace sim