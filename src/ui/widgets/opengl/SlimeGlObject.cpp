
#include "SlimeGlObject.hpp"
#include "ui/widgets/opengl/Slime.hpp"
#include <cassert>
#include <epoxy/gl_generated.h>

namespace ui::widgets::opengl {
SlimeGlObject::SlimeGlObject(std::shared_ptr<std::vector<opengl::Slime>>& slimes, float width, float height) : width(width),
                                                                                                               height(height),
                                                                                                               slimes(slimes) {}

void SlimeGlObject::set_slime_texture(GLuint slimeTexture) {
    this->slimeTexture = slimeTexture;
}

void SlimeGlObject::init_internal() {
    // Slime data:
    assert(slimes);
    glGenBuffers(1, &slimeSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, slimeSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(sizeof(opengl::Slime) * slimes->size()), static_cast<void*>(slimes->data()), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, slimeSSBO);

    // Compile shader:
    compShader = compile_shader("/ui/shader/slime/slime.comp", GL_COMPUTE_SHADER);
    assert(compShader > 0);

    // Prepare program:
    glAttachShader(shaderProg, compShader);
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
        glDetachShader(shaderProg, compShader);
    }
    GLERR;

    // Bind attributes:
    glUniform1i(glGetUniformLocation(shaderProg, "slimeTexture"), 0);
    GLERR;
}

void SlimeGlObject::render_internal() {
    assert(slimes);

    glBindImageTexture(0, slimeTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    GLERR;

    // Execute the compute shader:
    glDispatchCompute(static_cast<GLuint>(slimes->size()), 1, 1);
    GLERR;
    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    GLERR;
}

void SlimeGlObject::cleanup_internal() {
    glDeleteShader(compShader);
}
}  // namespace ui::widgets::opengl