#ifndef APP_H
#define APP_H

#include "client.hpp"
#include "ui.hpp"

static constexpr int deltaTime = 3;

struct cpuinfo {

};

struct psinfo {

}

struct appinfo {
    cpuinfo cpus[8];
    psinfo* processes;
    double uptime;
}

class App {
public:
    App() = default;
    init();
    main_loop();

private:
    static 
}

#endif
