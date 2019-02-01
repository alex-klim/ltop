#include <termbox.h>
#include <iostream>
#include <vector>

#include "client.hpp"
#include "ui.hpp"
#include "app.hpp"

int main() {
    App app;
    app.init();
    app.main_loop();
    app.shutdown();
    return 0;
}
