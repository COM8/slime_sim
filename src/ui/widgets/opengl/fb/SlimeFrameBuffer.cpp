#include "SlimeFrameBuffer.hpp"
#include "ui/widgets/opengl/fb/AbstractGlFrameBuffer.hpp"

namespace ui::widgets::opengl::fb {

SlimeFrameBuffer::SlimeFrameBuffer(GLsizei sizeX, GLsizei sizeY) : AbstractGlFrameBuffer(sizeX, sizeY) {}

void SlimeFrameBuffer::init_internal() {}

void SlimeFrameBuffer::bind_internal() {}

void SlimeFrameBuffer::cleanup_internal() {}

}  // namespace ui::widgets::opengl::fb