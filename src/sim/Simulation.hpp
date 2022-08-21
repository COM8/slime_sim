#pragma once

#include "Slime.hpp"
#include "Species.hpp"
#include <memory>
#include <vector>

namespace sim {
class Simulation {
 private:
    std::shared_ptr<std::vector<Slime>> slimes{std::make_shared<std::vector<Slime>>()};
    std::shared_ptr<std::vector<Species>> species{std::make_shared<std::vector<Species>>()};

    bool initialized{false};
    bool running{false};
    bool blurEnables{true};

    size_t width{2560};
    size_t height{1440};
    const size_t INITIAL_NUM_SLIMES{10000};

    void init();
    [[nodiscard]] bool is_initialized() const;

 public:
    [[nodiscard]] const std::shared_ptr<std::vector<Slime>>& get_slimes() const;
    [[nodiscard]] const std::shared_ptr<std::vector<Species>>& get_species() const;

    [[nodiscard]] bool is_running() const;
    [[nodiscard]] bool is_blur_enabled() const;
    [[nodiscard]] size_t get_width() const;
    [[nodiscard]] size_t get_height() const;

    void set_running(bool running);
    void set_blur_enabled(bool blurEnables);

    static std::shared_ptr<Simulation>& get_instance();
};
}  // namespace sim