#pragma once

#include "Types.hpp"
#include <cstdint>

namespace sim {
struct Species {
    Rgba color{1.0, 0.0, 0.0, 1.0};
    float moveSpeed{0};
    float turnSpeed{0};

    float sensorAngleDegrees{0};
    float sensorOffsetDst{0};
    int sensorSize{0};

 private:
    float padding0{0};
    Vec2 padding1{};
    Vec4U padding2{};

 public:
    Species(Rgba&& color, float moveSpeed, float turnSpeed, float sensorAngleDegrees, float sensorOffsetDst, int sensorSize);
} __attribute__((aligned(64))) __attribute__((__packed__));
}  // namespace sim