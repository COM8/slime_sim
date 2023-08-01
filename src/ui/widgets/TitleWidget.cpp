#include "TitleWidget.hpp"
#include <cairomm/pattern.h>
#include <pangomm/fontdescription.h>

namespace ui::widgets {
TitleWidget::TitleWidget() {
    prep_widget();
}

void TitleWidget::prep_widget() {
    set_draw_func(sigc::mem_fun(*this, &TitleWidget::on_draw_handler));
    set_can_target(false);
    set_can_focus(false);

    set_halign(Gtk::Align::FILL);
    set_valign(Gtk::Align::FILL);
    set_margin(10);

    set_overflow(Gtk::Overflow::HIDDEN);
}

void TitleWidget::draw_text(const std::string& text, const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height, int padding) {
    ctx->save();
    Pango::FontDescription font;
    font.set_stretch(Pango::Stretch::ULTRA_EXPANDED);
    font.set_weight(Pango::Weight::BOLD);
    Glib::RefPtr<Pango::Layout> layout = create_pango_layout(text);
    layout->set_font_description(font);
    ctx->move_to(padding, padding);

    int textWidth{0};
    int textHeight{0};
    layout->get_pixel_size(textWidth, textHeight);

    double widthRatio = static_cast<double>(width) / textWidth;
    double heightRatio = static_cast<double>(height) / textHeight;
    double ratio = std::min(widthRatio, heightRatio);

    // Scale the context by the ratio
    ctx->scale(ratio, ratio);

    // Draw font
    Gdk::RGBA foreground = get_style_context()->get_color();
    ctx->set_source_rgba(foreground.get_red(), foreground.get_green(), foreground.get_blue(), foreground.get_alpha());
    layout->show_in_cairo_context(ctx);

    ctx->restore();

    // Underline rectangle
    ctx->save();
    Glib::RefPtr<Cairo::LinearGradient> pattern = Cairo::LinearGradient::create(0, 0, width, height);
    pattern->add_color_stop_rgb(0, 1, 0.482352941, 0);
    pattern->add_color_stop_rgb(1, 1, 0.717647059, 0);
    ctx->set_source(pattern);

    ctx->rectangle(padding, static_cast<float>(textHeight) / 2 * heightRatio - 50, width - (padding * 2), textHeight * heightRatio * 0.015);
    ctx->fill();

    ctx->restore();
}

//-----------------------------Events:-----------------------------
void TitleWidget::on_draw_handler(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height) {
    const int padding = 40;
    draw_text("Simulating Boids and Slimes to\nUnderstand how GPUs Work", ctx, width - (padding * 2), height - (padding * 2), padding);
}
}  // namespace ui::widgets