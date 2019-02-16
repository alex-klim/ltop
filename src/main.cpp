#include "app.hpp"

int main() {
    App app;
    try {
        app.init();
        app.main_loop();
    } catch (...) {
        app.cleanup();
        std::cerr << "Something went wrong, but i dont want to mess up the terminal";
        return 0;
    }
    return 0;
}
