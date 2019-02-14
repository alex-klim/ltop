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
    std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));
}

// collecting data for each process
void App::collect_proclist() {
    fs::path proc("/proc");
    pd.clear();
    {
        std::lock_guard<std::mutex> lock(p_mutex);
        if (fs::is_directory(proc)) {
            for (auto& p: fs::directory_iterator(proc)) {
                std::string ogo(p.path().filename());
                if (is_num(ogo)) {
                    pd.push_back(proc_data());
                    std::string filename = proc.string()+'/'+ogo+"/stat";
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
        if (Ui::currentLine >= pd.size()) {
            Ui::currentLine = pd.size()-1;
        }
        ui->drawAll(Point(1,1), news, *md, pd);
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
                        kill(pd[Ui::currentLine].pid, SIGTERM);
                        draw();
                        break;
                    case TB_KEY_ARROW_DOWN: // scrolling down the process list
                        {                   // need to backup mutex for process container
                            std::lock_guard<std::mutex> lock(p_mutex);
                            if (Ui::currentLine < pd.size()-1) {
                                if (Ui::firstToDraw == Ui::currentLine
                                        - ui->get_height()+Ui::nCpus+6) {
                                    Ui::firstToDraw++;
                                }
                                Ui::currentLine++;
                            }
                        }
                        draw();
                        break;
                    case TB_KEY_ARROW_UP: // scrolling up the process list
                        if (Ui::currentLine > 0) {
                            if (Ui::currentLine == Ui::firstToDraw) {
                                Ui::firstToDraw--;
                            }
                            Ui::currentLine--;
                        }
                        draw();
                        break;
                }
                break;
            case TB_EVENT_RESIZE:
                ui->set_height();
                ui->set_width();
                if (Ui::currentLine - Ui::firstToDraw > ui->get_height()-Ui::nCpus-6) {
                    Ui::currentLine = Ui::firstToDraw+ui->get_height()-Ui::nCpus-6;
                }
                draw();
                break;
        }
    }
done:
    ui->on_exit();
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
