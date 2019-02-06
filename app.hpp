#ifndef APP_H
#define APP_H

#include "client.hpp"
#include "ui.hpp"
#include "containers.hpp"

#include <memory>
#include <future>
#include <mutex>
#include <iostream>
#include <vector>
#include <experimental/filesystem>

static constexpr int deltaTime = 2000; // milliseconds

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

class App {
public:
    App();
    ~App();
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    void init();
    void draw();
    void collect_data();
    void collect_proclist();
    int ui_loop(); // method to be asynced
    int main_loop();

private:
//    using std::vector< std::unique_ptr<proc_data> > = VectorUpDataT;
    std::unique_ptr<ginfo> gd;
    std::unique_ptr<minfo> md;
    std::vector<proc_data> pd;

    std::unique_ptr<Client> cl;
    std::unique_ptr<Ui> ui;
    std::mutex d_mutex;
    std::mutex p_mutex;
};

#endif // APP_H
