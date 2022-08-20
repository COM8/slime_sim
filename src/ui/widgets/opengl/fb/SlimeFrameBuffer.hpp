#pragma once

#include "AbstractGlFrameBuffer.hpp"
#include "SlimeFrameBuffer.hpp"
#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>

namespace ui::widgets::opengl::fb {
class SlimeFrameBuffer : public AbstractGlFrameBuffer {
 public:
    SlimeFrameBuffer(GLsizei sizeX, GLsizei sizeY);
    SlimeFrameBuffer(SlimeFrameBuffer& other) = delete;
    SlimeFrameBuffer(SlimeFrameBuffer&& old) = delete;

    ~SlimeFrameBuffer() override = default;

    SlimeFrameBuffer& operator=(SlimeFrameBuffer& other) = delete;
    SlimeFrameBuffer& operator=(SlimeFrameBuffer&& old) = delete;

 protected:
    void init_internal() override;
    void bind_internal() override;
    void cleanup_internal() override;
};
}  // namespace ui::widgets::opengl::fb