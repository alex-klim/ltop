#ifndef APP_CPP
#define APP_CPP

#include "app.hpp"
#include "utils.cpp"

#include <chrono>
#include <thread>

namespace fs = std::experimental::filesystem;

void cpu_cpy(cpu_info* dst, cpu_info* src) {
    for (auto i = 0; i < 9; ++i) {
        dst[i] = src[i];
    }
}

void calc_usage(cpu_info* lhs, cpu_info* rhs, double usage[4]) {
    ull all;
    ull work;
    for (auto i = 1; i < 5; ++i) {
        ull all = (rhs[i].user + rhs[i].system + rhs[i].nice
                + rhs[i].idle + rhs[i].iowait + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice
                + lhs[i].idle + lhs[i].iowait + lhs[i].irq + lhs[i].sirq);
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
    std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));
}

void App::collect_proclist() {
    fs::path proc("/proc");

    if (fs::is_directory(proc)) {
        for (auto& p: fs::directory_iterator(proc)) {
            std::string ogo(p.path().filename());
            if (is_num(ogo)) {
                pd.push_back(proc_data());
                cl->procstat(proc.string()+'/'+ogo+"/stat", &(pd.back()));
            }
        }
    }
}

void App::collect_data() {
    cpu_cpy(gd->last, gd->cur);
    cl->uptime(gd->uptime, gd->idle);
    cl->loadavg(gd->load[0], gd->load[1], gd->load[2], gd->running, gd->threads);
    cl->meminfo(md->memtot, md->memfree, md->memav, md->stot, md->sfree);
    cl->stat(gd->cur);
    calc_usage(gd->last, gd->cur, gd->usage);
    collect_proclist();
}

int App::ui_loop() {
    ui->set_width();
    ui->set_height();
    draw();

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
                draw();
                break;
        }
    }
done:
    ui->on_exit();
    return 0;
}

void App::draw() {
    {
        std::lock_guard<std::mutex> lock(d_mutex);
        data news{
            gd->uptime,
            gd->idle,
            { *(gd->load), *(gd->load+1), *(gd->load+2) },
            { *(gd->usage), *(gd->usage+1), *(gd->usage+2), *(gd->usage+3) },
            gd->threads,
            gd->running
        };
        ui->drawAll(Point(1,2), news, pd);
    }
}

int App::main_loop() {
    auto f = std::async(std::launch::async, &App::ui_loop, this);
    while(1) {
        {
            std::lock_guard<std::mutex> lock(d_mutex);
            collect_data();
        }
        if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            try {
                auto a = f.get();
            } catch (std::exception& e) {
                std::cerr << "[ui thread finished]: " << e.what() <<'\n';
                return -1;
            }
            break;
        }
        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));
    }
    return 0;
}

#endif // APP_CPP
