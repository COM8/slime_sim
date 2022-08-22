#include "Simulation.hpp"
#include <cmath>
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
    species->emplace_back(Species(Rgba::random_color(), 1, 0.25, 30, 35, 1));
    for (size_t i = 0; i < INITIAL_NUM_SLIMES / 2; i++) {
        slimes->emplace_back(Slime(
            Vec4U::random_vec(),
            Vec2::random_vec(static_cast<float>(width) / 2, static_cast<float>(width), 0, static_cast<float>(height)),
            0,
            Slime::random_angle()));
    }

    species->emplace_back(Species(Rgba::random_color(), 1, 0.05, 20, 35, 1));
    for (size_t i = 0; i < INITIAL_NUM_SLIMES / 2; i++) {
        slimes->emplace_back(Slime(
            Vec4U::random_vec(),
            Vec2::random_vec(0, static_cast<float>(width) / 2, 0, static_cast<float>(height)),
            1,
            Slime::random_angle()));
    }

    initialized = true;
}

bool Simulation::is_running() const {
    return running;
}

bool Simulation::is_blur_enabled() const {
    return blurEnables;
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
