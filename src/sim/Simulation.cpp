#include "Simulation.hpp"
#include "sim/Types.hpp"
#include <cmath>
#include <numbers>
#include <random>

namespace sim {
const std::shared_ptr<std::vector<Slime>>& Simulation::get_slimes() const {
    return slimes;
}

const std::shared_ptr<std::vector<Species>>& Simulation::get_species() const {
    return species;
}

std::shared_ptr<Simulation>& Simulation::get_instance() {
    static std::shared_ptr<Simulation> instance = std::make_shared<Simulation>();
    if (!instance->is_initialized()) {
        instance->init();
    }
    return instance;
}

bool Simulation::is_initialized() const {
    return initialized;
}

void Simulation::init() {
    // Default:
    const size_t SPECIES_COUNT = 1;
    for (size_t i = 0; i < SPECIES_COUNT; i++) {
        species->emplace_back(Species(Rgba::random_color(), 1, 0.15, 30, 10, 2));
        float spawnStartX = (static_cast<float>(width) / static_cast<float>(SPECIES_COUNT)) * static_cast<float>(i);
        float spawnEndY = (static_cast<float>(width) / static_cast<float>(SPECIES_COUNT)) * static_cast<float>(i + 1);

        for (size_t e = 0; e < INITIAL_NUM_SLIMES / SPECIES_COUNT; e++) {
            slimes->emplace_back(Slime(
                Vec4U::random_vec(),
                Vec2::random_vec(spawnStartX, spawnEndY, 0, static_cast<float>(height)),
                i,
                Slime::random_angle()));
        }
    }

    // Fancy circle
    /*const size_t SPECIES_COUNT = 2;
    for (size_t i = 0; i < SPECIES_COUNT; i++) {
        species->emplace_back(Species(Rgba::random_color(), 1, 0.01, 30, 10, 2));

        for (size_t e = 0; e < INITIAL_NUM_SLIMES / SPECIES_COUNT; e++) {
            float angleRadian = static_cast<float>(((2.0F * std::numbers::pi) / INITIAL_NUM_SLIMES) * static_cast<double>(e));
            float spawnX = (static_cast<float>(width) / 2) + (100 * std::sin(angleRadian));
            float spawnY = (static_cast<float>(height) / 2) + (100 * std::cos(angleRadian));
            slimes->emplace_back(Slime(
                Vec4U::random_vec(),
                Vec2{spawnX, spawnY},
                i,
                angleRadian));
        }
    }*/

    // Round slime distribution
    /*const size_t SPECIES_COUNT = 1;
    for (size_t i = 0; i < SPECIES_COUNT; i++) {
        species->emplace_back(Species(Rgba::random_color(), 1, 0.15, 30, 10, 2));

        for (size_t e = 0; e < INITIAL_NUM_SLIMES / SPECIES_COUNT; e++) {
            float angleRadian = static_cast<float>(((2.0F * std::numbers::pi) / INITIAL_NUM_SLIMES) * static_cast<double>(e));
            float spawnX = (static_cast<float>(width) / 2) + (100 * std::sin(angleRadian));
            float spawnY = (static_cast<float>(height) / 2) + (100 * std::cos(angleRadian));
            slimes->emplace_back(Slime(
                Vec4U::random_vec(),
                Vec2{spawnX, spawnY},
                i,
                angleRadian));
        }
    }*/

    initialized = true;
}

bool Simulation::is_running() const {
    return running;
}

bool Simulation::is_blur_enabled() const {
    return blurEnables;
}

bool Simulation::is_color_change_enabled() const {
    return colorChange;
}

void Simulation::set_color_change_enabled(size_t enableColorChange) {
    colorChange = enableColorChange;
}

void Simulation::set_running(bool running) {
    this->running = running;
}

void Simulation::set_blur_enabled(bool blurEnables) {
    this->blurEnables = blurEnables;
}

size_t Simulation::get_width() const {
    return width;
}

size_t Simulation::get_height() const {
    return height;
}

void Simulation::set_species_need_sync(bool speciesNeedSync) {
    this->speciesNeedSync = speciesNeedSync;
}

void Simulation::set_slimes_need_sync(bool slimesNeedSync) {
    this->slimesNeedSync = slimesNeedSync;
}

bool Simulation::do_species_need_sync() const {
    return speciesNeedSync;
}

bool Simulation::do_slimes_need_sync() const {
    return slimesNeedSync;
}

void Simulation::add_slimes(float x, float y, size_t count, uint32_t speciesIndex) {
    static std::random_device device;
    static std::mt19937 gen(device());
    static std::uniform_real_distribution<float> distr(0, 1.0);
    for (size_t i = 0; i < count; i++) {
        x += (distr(gen) * 50) - 25;
        x = std::min(x, static_cast<float>(width));
        x = std::max(x, static_cast<float>(0));

        y += (distr(gen) * 50) - 25;
        y = std::min(y, static_cast<float>(height));
        y = std::max(y, static_cast<float>(0));

        slimes->emplace_back(Slime(
            Vec4U::random_vec(),
            Vec2{x, y},
            speciesIndex,
            Slime::random_angle()));
    }
    slimesNeedSync = true;
}

}  // namespace sim
