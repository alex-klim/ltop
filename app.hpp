#ifndef APP_H
#define APP_H

#include "client.hpp"
#include "ui.hpp"

#include <memory>
#include <future>
#include <mutex>

static constexpr int deltaTime = 3000; // milliseconds

struct minfo {
    ull memtot;
    ull memfree;
    ull memav;
    ull stot;
    ull sfree;
};

using ginfo = struct {
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
    void collect_data();
    static void stat_loop();
    void main_loop();

private:
    static ginfo* gd;
    static minfo* md;
    std::unique_ptr<Client> cl;
    std::unique_ptr<Ui> ui;
    std::mutex d_mutex;
};

#endif
