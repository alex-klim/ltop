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

void calc_usage(cpu_info* lhs, cpu_info* rhs, double* usage) {
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

App::App() {
    gd = new ginfo;
    md = new minfo;
}

App::~App() {
    delete gd;
    delete md;
}

void App::init() {
    collect_data();
}

void App::collect_data() {
    cpu_cpy(gd->last, gd->cur);
    cl->uptime(gd->uptime, gd->idle);
    cl->loadavg(gd->load[0], gd->load[1], gd->load[2], gd->running, gd->threads);
    cl->meminfo(md->memtot, md->memfree, md->memav, md->stot, md->sfree);
    cl->stat(gd->cur);
    calc_usage(gd->last, gd->cur, gd->usage);
}

void App::stat_loop() {
    while(1) {
        collect_data();
        {
            std::lock_guard<std::mutex> lock(d_mutex);
            ui->drawAll(
                    gd->uptime,
                    gd->idle,
                    gd->load,
                    gd->usage,
                    gd->threads,
                    gd->running);
                }
        std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));
    }
}

void App::main_loop() {
    auto f = std::async(std::launch::async, stat_loop);
    Ui::set_width();
    Ui::set_height();
    ui->drawAll({
            gd->uptime,
            gd->idle,
            gd->load,
            gd->usage,
            gd->threads,
            gd->running
            });

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
                Ui::set_height();
                Ui::set_width();
                ui->drawAll({
                    gd->uptime,
                    gd->idle,
                    gd->load,
                    gd->usage,
                    gd->threads,
                    gd->running
                });
                break;
        }
        if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            try {
                auto a = f.get();
            } catch (std::exception& e) {
                std::cerr << "[client error]: " << e.what() <<'\n';
                goto done;
            }
            goto done;
        }
    }
done:
    return;
}

#endif // APP_CPP
