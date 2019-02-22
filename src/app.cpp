#ifndef APP_CPP
#define APP_CPP

#include "app.hpp"
#include "utils.h"

#include <chrono>
#include <thread>
#include <signal.h>

namespace fs = std::experimental::filesystem;

App::App() : gd(new ginfo()), md(new minfo()), cl(new Client()), ui(new Ui()) {}

App::~App() {}

// collecting info on startup, so ui could show us some useful information
void App::init() {
    ui->init();
    collect_data();
}

void App::cleanup() const {
    ui->on_exit();
}

// collecting data for each process
void App::collect_proclist() {
    fs::path proc("/proc");
    pd.clear();
    {
        std::lock_guard<std::mutex> lock(p_mutex);
        if (fs::is_directory(proc)) {
            for (auto& p: fs::directory_iterator(proc)) {
                std::string dir(p.path().filename());
                if (is_num(dir)) {
                    pd.push_back(proc_data());
                    std::string filename = proc.string()+'/'+dir+"/stat";
                    cl->procstat(filename, &(pd.back()));
                    cl->procstatus(filename+"us", &(pd.back()));
                }
            }
        }
    }
}

// collecting processor and memory info
void App::collect_data() {
    cpu_cpy(gd->last, gd->cur);
    cl->uptime(gd->uptime, gd->idle);
    cl->loadavg(gd->load[0], gd->load[1], gd->load[2], gd->running, gd->threads);
    cl->meminfo(md->memtot, md->memfree, md->memav, md->stot, md->sfree);
    cl->stat(gd->cur);
    calc_usage(gd->last, gd->cur, gd->usage);
    collect_proclist();
}

void App::draw() {
    {
        std::lock_guard<std::mutex> lock(d_mutex);
        std::lock_guard<std::mutex> lock2(p_mutex);
        data news{
            gd->uptime,
            gd->idle,
            { *(gd->load), *(gd->load+1), *(gd->load+2) },
            { *(gd->usage), *(gd->usage+1), *(gd->usage+2), *(gd->usage+3) },
            gd->threads,
            gd->running
        };
        if (ui->currentLine >= pd.size()) {
            ui->currentLine = pd.size()-1;
        }
        ui->drawAll(news, *md, pd);
    }
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
                    case TB_KEY_F9:
                        kill(pd[ui->currentLine].pid, SIGTERM);
                        draw();
                        break;
                    case TB_KEY_ARROW_DOWN: // scrolling down the process list
                        {                   // need to backup mutex for process container
                            std::lock_guard<std::mutex> lock(p_mutex);
                            if (ui->currentLine < pd.size()-1) {
                                if (ui->firstToDraw == ui->currentLine
                                        - (ui->get_height()-1)+ui->headerPadding+1) {
                                    ui->firstToDraw++;
                                }
                                ui->currentLine++;
                            }
                        }
                        draw();
                        break;
                    case TB_KEY_ARROW_UP: // scrolling up the process list
                        if (ui->currentLine > 0) {
                            if (ui->currentLine == ui->firstToDraw) {
                                ui->firstToDraw--;
                            }
                            ui->currentLine--;
                        }
                        draw();
                        break;
                }
                break;
            case TB_EVENT_RESIZE:
                ui->set_height();
                ui->set_width();
                if (ui->currentLine - ui->firstToDraw > (ui->get_height()-1)-ui->headerPadding-1) {
                    ui->currentLine = ui->firstToDraw+(ui->get_height()-1)-ui->headerPadding-1;
                }
                draw();
                break;
        }
    }
done:
    cleanup();
    return 0;
}

int App::main_loop() {
    auto f = std::async(std::launch::async, &App::ui_loop, this);
    // client loop
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
