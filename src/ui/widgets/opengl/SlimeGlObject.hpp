#pragma once

#include "AbstractGlObject.hpp"
#include "sim/Simulation.hpp"
#include <memory>
#include <vector>
#include <epoxy/gl.h>

namespace ui::widgets::opengl {
class SlimeGlObject : public AbstractGlObject {
 private:
    GLuint compShader{0};

    GLuint slimeTexture{0};
    GLuint slimeSSBO{0};
    GLuint speciesSSBO{0};
    GLuint attribSSBO{0};

    std::shared_ptr<sim::Simulation> simulation{sim::Simulation::get_instance()};

 public:
    SlimeGlObject() = default;
    SlimeGlObject(SlimeGlObject& other) = delete;
    SlimeGlObject(SlimeGlObject&& old) = delete;

    ~SlimeGlObject() override = default;

    SlimeGlObject& operator=(SlimeGlObject& other) = delete;
    SlimeGlObject& operator=(SlimeGlObject&& old) = delete;

    void set_slime_texture(GLuint slimeTexture);

 protected:
    void init_internal() override;
    void render_internal() override;
    void cleanup_internal() override;
};
}  // namespace ui::widgets::opengl
