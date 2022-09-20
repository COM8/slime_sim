#pragma once

#include "AbstractGlObject.hpp"
#include <array>
#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>
#include <gtkmm/glarea.h>

namespace ui::widgets::opengl {
class SpeciesPreviewGlObject : public AbstractGlObject {
 private:
    GLuint vertShader{0};
    GLuint geomShader{0};
    GLuint fragShader{0};

 public:
    SpeciesPreviewGlObject() = default;
    SpeciesPreviewGlObject(SpeciesPreviewGlObject& other) = delete;
    SpeciesPreviewGlObject(SpeciesPreviewGlObject&& old) = delete;

    ~SpeciesPreviewGlObject() override = default;

    SpeciesPreviewGlObject& operator=(SpeciesPreviewGlObject& other) = delete;
    SpeciesPreviewGlObject& operator=(SpeciesPreviewGlObject&& old) = delete;

 protected:
    void init_internal() override;
    void render_internal() override;
    void cleanup_internal() override;
};
}  // namespace ui::widgets::opengl
