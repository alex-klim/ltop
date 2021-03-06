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
#include <array>

static constexpr int deltaTime = 2000; // milliseconds

class App {
public:
    App();
    ~App();
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    void init();
    void cleanup() const;
    void draw();
    void collect_data();
    void collect_proclist();
    int ui_loop(); // method for ui loop
    int main_loop(); // method for gathering procfs info

private:
    std::unique_ptr<ginfo> gd;
    std::unique_ptr<minfo> md;
    std::vector<proc_data> pd;

    std::unique_ptr<Client> cl;
    std::unique_ptr<Ui> ui;
    std::mutex d_mutex; // mutex for syncing global data
    std::mutex p_mutex; // mutex for syncing process data
};

#endif // APP_H
