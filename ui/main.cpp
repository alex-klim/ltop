#include "include/ui.hpp"
#include <iostream>

#include <chrono>
#include <thread>



int main() {
    data news{
        240,
        1,
        {0.12, 0.25, 0.31},
        12400
    };
    Ui ui;
    ui.init();
    ui.ui_loop(news);
    tb_present();
    tb_clear();
    tb_shutdown();
    return 0;
}
