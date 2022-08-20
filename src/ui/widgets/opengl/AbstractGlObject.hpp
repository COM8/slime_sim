#pragma once

#include "logger/Logger.hpp"
#include "ui/widgets/opengl/utils/Utils.hpp"
#include <memory>
#include <string>
#include <epoxy/gl.h>

namespace ui::widgets::opengl {
class AbstractGlObject {
 protected:
    GLuint vao{0};
    GLuint vbo{0};

    GLuint shaderProg{0};

 public:
    AbstractGlObject() = default;
    AbstractGlObject(AbstractGlObject& other) = delete;
    AbstractGlObject(AbstractGlObject&& old) = delete;

    virtual ~AbstractGlObject() = default;

    AbstractGlObject& operator=(AbstractGlObject& other) = delete;
    AbstractGlObject& operator=(AbstractGlObject&& old) = delete;

 protected:
    virtual void init_internal() = 0;
    virtual void render_internal() = 0;
    virtual void cleanup_internal() = 0;

    static GLuint compile_shader(const std::string& resourcePath, GLenum type);

 public:
    void init();
    void render();
    void cleanup();
};
}  // namespace ui::widgets::opengl
