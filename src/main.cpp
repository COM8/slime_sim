#include "logger/Logger.hpp"
#include "ui/UiContext.hpp"

int main(int argc, char** argv) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version {} {} in UI mode.", SLIME_SIMULATOR_VERSION, SLIME_SIMULATOR_VERSION_NAME);

    // The UI context manages everything that is UI related.
    // It will return once all windows have been terminated.
    ui::UiContext ui;
    return ui.run(argc, argv);
}