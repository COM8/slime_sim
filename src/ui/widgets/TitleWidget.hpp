#pragma once

#include <gtkmm/drawingarea.h>

namespace ui::widgets {
class TitleWidget : public Gtk::DrawingArea {
 public:
    TitleWidget();
    TitleWidget(TitleWidget&&) = delete;
    TitleWidget(const TitleWidget&) = delete;
    TitleWidget& operator=(TitleWidget&&) = delete;
    TitleWidget& operator=(const TitleWidget&) = delete;
    ~TitleWidget() override = default;

 private:
    void prep_widget();
    void draw_text(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height, int padding);

    //-----------------------------Events:-----------------------------
    void on_draw_handler(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height);
};
}  // namespace ui::widgets
