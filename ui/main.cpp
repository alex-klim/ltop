#include "include/ui.hpp"
#include <iostream>

#include <chrono>
#include <thread>

static constexpr int threads = 240;
static constexpr int running = 1;
static constexpr double load[3] = {0.12, 0.25, 0.31};
static constexpr unsigned long long uptime = 12400;

int main() {
    Ui ui;
    ui.init();
    ui.ui_loop();
    tb_present();
    tb_clear();
    tb_shutdown();
    return 0;
}
