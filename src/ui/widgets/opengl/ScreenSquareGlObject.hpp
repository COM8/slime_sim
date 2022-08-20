#pragma once

#include "AbstractGlObject.hpp"
#include <array>
#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>
#include <gtkmm/glarea.h>

namespace ui::widgets::opengl {
class ScreenSquareGlObject : public AbstractGlObject {
 private:
    GLuint vertShader{0};
    GLuint geomShader{0};
    GLuint fragShader{0};

    GLint screenSizeConst{0};
    GLint textureSizeConst{0};

    /**
     * All textures that should be passed to the shader.
     * frameBufferTextures[0] is the slimes texture.
     **/
    std::array<GLuint, 1> frameBufferTextures{};

    Gtk::GLArea* glArea{nullptr};

    const float width;
    const float height;

 public:
    ScreenSquareGlObject(float width, float height);
    ScreenSquareGlObject(ScreenSquareGlObject& other) = delete;
    ScreenSquareGlObject(ScreenSquareGlObject&& old) = delete;

    ~ScreenSquareGlObject() override = default;

    ScreenSquareGlObject& operator=(ScreenSquareGlObject& other) = delete;
    ScreenSquareGlObject& operator=(ScreenSquareGlObject&& old) = delete;

    void set_glArea(Gtk::GLArea* glArea);
    void bind_texture(GLuint slimeFrameBufferTexture);

 protected:
    void init_internal() override;
    void render_internal() override;
    void cleanup_internal() override;
};
}  // namespace ui::widgets::opengl
