
#include "SlimeGlObject.hpp"
#include "ui/widgets/opengl/Slime.hpp"
#include <cassert>
#include <epoxy/gl_generated.h>

namespace ui::widgets::opengl {
SlimeGlObject::SlimeGlObject(std::shared_ptr<std::vector<opengl::Slime>>& slimes, float width, float height) : width(width),
                                                                                                               height(height),
                                                                                                               slimes(slimes) {}

void SlimeGlObject::init_internal() {
    // Vertex data:
    assert(slimes);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(opengl::Slime) * slimes->size()), static_cast<void*>(slimes->data()), GL_DYNAMIC_DRAW);

    // Compile shader:
    vertShader = compile_shader("/ui/shader/slime/slime.vert", GL_VERTEX_SHADER);
    assert(vertShader > 0);
    geomShader = compile_shader("/ui/shader/slime/slime.geom", GL_GEOMETRY_SHADER);
    assert(geomShader > 0);
    fragShader = compile_shader("/ui/shader/slime/slime.frag", GL_FRAGMENT_SHADER);
    assert(fragShader > 0);

    // Prepare program:
    glAttachShader(shaderProg, vertShader);
    glAttachShader(shaderProg, geomShader);
    glAttachShader(shaderProg, fragShader);
    glBindFragDataLocation(shaderProg, 0, "outColor");
    glLinkProgram(shaderProg);
    GLERR;

    // Check for errors during linking:
    GLint status = GL_FALSE;
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len = 0;
        glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &log_len);

        std::string log_msg;
        log_msg.resize(log_len);
        glGetProgramInfoLog(shaderProg, log_len, nullptr, static_cast<GLchar*>(log_msg.data()));
        SPDLOG_ERROR("Linking slime shader program failed: {}", log_msg);
        glDeleteProgram(shaderProg);
        shaderProg = 0;
    } else {
        glDetachShader(shaderProg, fragShader);
        glDetachShader(shaderProg, geomShader);
        glDetachShader(shaderProg, vertShader);
    }
    GLERR;

    // Bind attributes:
    glUseProgram(shaderProg);
    GLint colAttrib = glGetAttribLocation(shaderProg, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Slime), nullptr);

    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glEnableVertexAttribArray(posAttrib);
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Slime), reinterpret_cast<void*>(sizeof(Rgba) + sizeof(Vec4U)));

    worldSizeConst = glGetUniformLocation(shaderProg, "worldSize");
    glUniform2f(worldSizeConst, width, height);

    rectSizeConst = glGetUniformLocation(shaderProg, "rectSize");
    glUniform2f(rectSizeConst, 10, 10);
    GLERR;
}

void SlimeGlObject::render_internal() {
    assert(slimes);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(slimes->size()));
}

void SlimeGlObject::cleanup_internal() {
    glDeleteShader(fragShader);
    glDeleteShader(geomShader);
    glDeleteShader(fragShader);
}
}  // namespace ui::widgets::opengl