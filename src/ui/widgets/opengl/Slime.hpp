#pragma once

#include <cstdint>
namespace ui::widgets::opengl {
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

struct Slime {
    Vec4U randState{};
    Vec2 pos{};
    uint32_t speciesIndex{0};
    float angle{0};

 public:
    Slime(Vec4U&& randState, Vec2&& pos, uint32_t speciesIndex, float angle);

    static float random_angle();
} __attribute__((aligned(32))) __attribute__((__packed__));

struct Species {
    Rgba color{1.0, 0.0, 0.0, 1.0};
    float moveSpeed{0};
    float turnSpeed{0};

    float sensorAngleDegrees{0};
    float sensorOffsetDst{0};
    int sensorSize{0};

    float padding0{0};
    Vec2 padding1{};
    Vec4U padding2{};

    Species(Rgba&& color, float moveSpeed, float turnSpeed, float sensorAngleDegrees, float sensorOffsetDst, int sensorSize);
} __attribute__((aligned(64))) __attribute__((__packed__));
}  // namespace ui::widgets::opengl