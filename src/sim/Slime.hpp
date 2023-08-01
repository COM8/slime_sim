#pragma once

#include "Types.hpp"
#include <cstdint>

namespace sim {
struct Slime {
    Vec4U randState{};
    Vec2 pos{};
    uint32_t speciesIndex{0};
    float angle{0};

    Slime(Vec4U&& randState, Vec2&& pos, uint32_t speciesIndex, float angle);

    static float random_angle();
} __attribute__((aligned(32))) __attribute__((__packed__));
}  // namespace sim