#pragma once

#include "AbstractGlObject.hpp"
#include "ui/widgets/opengl/Slime.hpp"
#include <memory>
#include <vector>
#include <epoxy/gl.h>

namespace ui::widgets::opengl {
class SlimeGlObject : public AbstractGlObject {
 private:
    GLuint vertShader{0};
    GLuint geomShader{0};
    GLuint fragShader{0};

    GLint worldSizeConst{0};
    GLint rectSizeConst{0};

    const float width;
    const float height;

    std::shared_ptr<std::vector<opengl::Slime>> slimes{nullptr};

 public:
    explicit SlimeGlObject(std::shared_ptr<std::vector<opengl::Slime>>& slimes, float width, float height);
    SlimeGlObject(SlimeGlObject& other) = delete;
    SlimeGlObject(SlimeGlObject&& old) = delete;

    ~SlimeGlObject() override = default;

    SlimeGlObject& operator=(SlimeGlObject& other) = delete;
    SlimeGlObject& operator=(SlimeGlObject&& old) = delete;

    void set_slimes(const std::shared_ptr<std::vector<opengl::Slime>>& slimes);

 protected:
    void init_internal() override;
    void render_internal() override;
    void cleanup_internal() override;
};
}  // namespace ui::widgets::opengl
