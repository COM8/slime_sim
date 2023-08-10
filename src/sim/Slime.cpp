#include "Slime.hpp"
#include <cstdint>
#include <limits>
#include <random>
#include <numbers>

namespace sim {
Slime::Slime(Vec4U&& randState, Vec2&& pos, uint32_t speciesIndex, float angle) : randState(randState),
                                                                                  pos(pos),
                                                                                  speciesIndex(speciesIndex),
                                                                                  angle(angle) {}

float Slime::random_angle() {
    static std::random_device device;
    static std::mt19937 gen(device());
    static std::uniform_real_distribution<float> distr(0, 1.0);

    return static_cast<float>(distr(gen) * 2.0 * std::numbers::pi);
}
}  // namespace sim