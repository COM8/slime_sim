
#include "SlimeGlObject.hpp"
#include "sim/Slime.hpp"
#include "sim/Species.hpp"
#include <cassert>
#include <cstdint>
#include <vector>
#include <epoxy/gl_generated.h>

namespace ui::widgets::opengl {
void SlimeGlObject::set_slime_texture(GLuint slimeTexture) {
    this->slimeTexture = slimeTexture;
}

void SlimeGlObject::init_internal() {
    // Slime data:
    assert(simulation);
    glGenBuffers(1, &slimeSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, slimeSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(sizeof(sim::Slime) * simulation->get_slimes()->size()), static_cast<void*>(simulation->get_slimes()->data()), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, slimeSSBO);

    glGenBuffers(1, &speciesSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, speciesSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(sizeof(sim::Species) * simulation->get_species()->size()), static_cast<void*>(simulation->get_species()->data()), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, speciesSSBO);

    // Attributes:
    glGenBuffers(1, &attribSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, attribSSBO);
    std::vector<int32_t> attr{static_cast<int32_t>(simulation->get_width()), static_cast<int32_t>(simulation->get_height())};
    glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(sizeof(int32_t) * attr.size()), static_cast<void*>(attr.data()), GL_STATIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, attribSSBO);

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
    assert(simulation);

    if (simulation->do_slimes_need_sync()) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, slimeSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(sizeof(sim::Slime) * simulation->get_slimes()->size()), static_cast<void*>(simulation->get_slimes()->data()), GL_DYNAMIC_DRAW);
        simulation->set_slimes_need_sync(false);
    }

    if (simulation->do_species_need_sync()) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, speciesSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(sizeof(sim::Species) * simulation->get_species()->size()), static_cast<void*>(simulation->get_species()->data()), GL_DYNAMIC_DRAW);
        simulation->set_species_need_sync(false);
    }

    glBindImageTexture(0, slimeTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    GLERR;

    // Execute the compute shader:
    glDispatchCompute(static_cast<GLuint>(simulation->get_slimes()->size()), 1, 1);
    GLERR;
    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    GLERR;
}

void SlimeGlObject::cleanup_internal() {
    glDeleteShader(compShader);
}
}  // namespace ui::widgets::opengl