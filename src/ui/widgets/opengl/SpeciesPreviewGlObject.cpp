
#include "SpeciesPreviewGlObject.hpp"
#include <cassert>
#include <epoxy/gl_generated.h>

namespace ui::widgets::opengl {
void SpeciesPreviewGlObject::init_internal() {
    // Compile shader:
    vertShader = compile_shader("/ui/shader/species_preview/species_preview.vert", GL_VERTEX_SHADER);
    assert(vertShader > 0);
    geomShader = compile_shader("/ui/shader/species_preview/species_preview.geom", GL_GEOMETRY_SHADER);
    assert(geomShader > 0);
    fragShader = compile_shader("/ui/shader/species_preview/species_preview.frag", GL_FRAGMENT_SHADER);
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
        SPDLOG_ERROR("Linking screen square shader program failed: {}", log_msg);
        glDeleteProgram(shaderProg);
        shaderProg = 0;
    } else {
        glDetachShader(shaderProg, fragShader);
        glDetachShader(shaderProg, vertShader);
        glDetachShader(shaderProg, geomShader);
    }
    GLERR;
}

void SpeciesPreviewGlObject::render_internal() {
    glDrawArrays(GL_POINTS, 0, 1);
    GLERR;
}

void SpeciesPreviewGlObject::cleanup_internal() {
    glDeleteShader(fragShader);
    glDeleteShader(geomShader);
    glDeleteShader(fragShader);
}
}  // namespace ui::widgets::opengl