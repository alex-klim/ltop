#ifndef APP_H
#define APP_H

#include "client.hpp"
#include "ui.hpp"

#include <memory>
#include <future>
#include <mutex>
#include <iostream>

static constexpr int deltaTime = 1000; // milliseconds

struct minfo {
    ull memtot;
    ull memfree;
    ull memav;
    ull stot;
    ull sfree;
};

struct ginfo {
    ull uptime;
    ull idle;
    double load[3];
    double usage[4];
    int threads;
    int running;
    cpu_info* last;
    cpu_info* cur;
};

/*struct appinfo {
    cpuinfo cpus[8];
    psinfo* processes;
    double uptime;
};*/

class App {
public:
    App();
    ~App();
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    void init();
    void draw();
    void collect_data();
    int ui_loop(); // method to be asynced
    int main_loop();

private:
    std::unique_ptr<ginfo> gd;
    std::unique_ptr<minfo> md;

    std::unique_ptr<Client> cl;
    std::unique_ptr<Ui> ui;
    std::mutex d_mutex;
};

#endif
