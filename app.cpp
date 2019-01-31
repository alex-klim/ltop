#ifndef APP_CPP
#define APP_CPP

#include "app.hpp"

#include <chrono>
#include <thread>

void cpu_cpy(cpu_info* lhs, cpu_info* rhs) {
    for (auto i = 0; i < 9; ++i) {
        lhs[i] = rhs[i];
    }
}

void calc_usage(cpu_info* lhs, cpu_info* rhs, double usage[4]) {
    ull all;
    ull work;
    for (auto i = 1; i < 5; ++i) {
        ull all = (rhs[i].user + rhs[i].system + rhs[i].nice + rhs[i].idle + rhs[i].iowait + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice + lhs[i].idle + lhs[i].iowait + lhs[i].irq + lhs[i].sirq);
        ull work = (rhs[i].user + rhs[i].system + rhs[i].nice + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice + lhs[i].irq + lhs[i].sirq);
        usage[i-1] = static_cast<double>(work)/all;
    }
}

App::App() : gd(new ginfo()), md(new minfo()), cl(new Client()), ui(new Ui()) {
    gd->last = new cpu_info[9]();
    gd->cur = new cpu_info[9]();
}

App::~App() {
    delete gd->last;
    delete gd->cur;
}

void App::init() {
    ui->init();
    collect_data();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void App::shutdown() {
    ui->on_exit();
}

void App::collect_data() {
    cpu_cpy(gd->last, gd->cur);
    cl->uptime(gd->uptime, gd->idle);
    cl->loadavg(gd->load[0], gd->load[1], gd->load[2], gd->running, gd->threads);
    cl->meminfo(md->memtot, md->memfree, md->memav, md->stot, md->sfree);
    cl->stat(gd->cur);
    calc_usage(gd->last, gd->cur, gd->usage);
}

int App::stat_loop() {
    data news{
        gd->uptime,
        gd->idle,
        { *(gd->load), *(gd->load+1), *(gd->load+2) },
        { *(gd->usage), *(gd->usage+1), *(gd->usage+2), *(gd->usage+3) },
        gd->threads,
        gd->running
    };
    while(1) {
        collect_data();
        {
            std::lock_guard<std::mutex> lock(d_mutex);
            ui->drawAll(Point(1,2), news);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));
    }
}

int App::main_loop() {
    data news{
        gd->uptime,
        gd->idle,
        { *(gd->load), *(gd->load+1), *(gd->load+2) },
        { *(gd->usage), *(gd->usage+1), *(gd->usage+2), *(gd->usage+3) },
        gd->threads,
        gd->running
    };
    auto f = std::async(std::launch::async, &App::stat_loop, this);
    ui->set_width();
    ui->set_height();
    ui->drawAll(Point(1,2), news);

    struct tb_event ev;
    while (tb_poll_event(&ev)) {
        switch (ev.type) {
            case TB_EVENT_KEY:
                switch (ev.key) {
                    case TB_KEY_ESC:
                        goto done;
                        break;
                }
                break;
            case TB_EVENT_RESIZE:
                ui->set_height();
                ui->set_width();
                ui->drawAll(Point(1,2), news);
                break;
        }
        if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            try {
                auto a = f.get();
            } catch (std::exception& e) {
                std::cerr << "[client error]: " << e.what() <<'\n';
                return -1;
            }
            goto done;
        }
    }
done:
    return 0;
}

#endif // APP_CPP
