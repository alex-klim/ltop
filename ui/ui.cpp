#include "include/ui.hpp"

#include <iostream>
#include <cstring>
#include <thread>


size_t Ui::firstToDraw = 0;
size_t Ui::nCpus = std::thread::hardware_concurrency();

void Ui::set_height() {
    w_height = tb_height();
}

void Ui::set_width() {
    w_width = tb_width();
}

int Ui::get_height() const {
    return w_height;
}

int Ui::get_width() const {
    return w_width;
}

void Ui::init() const {
    int rc = tb_init();
    if (rc < 0) {
        std::cerr << "Can't init graphics, shutting down..\n";
        return;
    }
    tb_select_input_mode(TB_INPUT_ESC);
    tb_select_output_mode(TB_OUTPUT_256);
}

void Ui::on_exit() const {
    tb_clear();
    tb_shutdown();
}

void Ui::drawCpuLoad(Point start, int maxx, int load) const {
    //std::cerr << "Max: " << maxx << '\n';
    tb_change_cell(start.x_, start.y_, '[', 151, 236);
    for (auto i = start.x_+1; i < load; i++) {
        tb_change_cell(i, start.y_, '=', 151, 236);
    }
    tb_change_cell(maxx, start.y_, ']', 151, 236);
}

void Ui::drawSeparator(int y) const {
    char* buff = new char[w_width-1];
    snprintf(buff, w_width-1,
            "%6s  %25s  %4s  %4s  %10s  %10s  %10s  %8s %c  %40s\0",
            "pid", "User", "PRI", "NI", "VIRT",
            "RES", "SHR", "TIME", 'S', "Process");
    for (size_t i = 0; i < strlen(buff); i++) {
        tb_change_cell(i+1, y, buff[i], 236, 151);
    }
    for (auto i = strlen(buff); i < w_width; ++i) {
        tb_change_cell(i, y, ' ', 151, 151);
    }
    delete buff;
}

void Ui::drawString(Point start, std::string& msg) const {
    //std::cerr << "===================================\n";
    //std::cerr << "Point: (" << start.x_ << ", " << start.y_ << ")\n";
    //std::cerr << msg <<'\n';
    //std::cerr << "===================================\n";
    for (size_t i = 0; i < msg.length(); i++) {
        tb_change_cell(start.x_+i, start.y_, msg[i], 151, 236);
    }
}

void Ui::drawSummary(Point start, double load[3], int threads, int running, ull uptime) const {
    //std::cerr << "Point: (" << start.x_ << ", " << start.y_ << ")\n";
    std::string buf = "Tasks: " + std::to_string(threads)
        + ", " + std::to_string(running) +" running";
    //std::cerr << buf <<'\n';
    drawString(start, buf);
    buf = "Load average: " + std::to_string(load[0]) + ", " + std::to_string(load[1])
        + ", " + std::to_string(load[2]);
    //std::cerr << buf <<'\n';
    drawString(Point(start.x_, start.y_+1), buf);
    buf = "Uptime: " + std::to_string(uptime);
    //std::cerr << buf <<'\n';
    drawString(Point(start.x_, start.y_+2), buf);
}

void Ui::drawStats(Point start, double usage[4]) const {
    int max_width = w_width/2;

    for (size_t i = 0; i < Ui::nCpus; i++) {
        drawCpuLoad(Point(start.x_, start.y_+i), max_width, (int)(usage[i]*max_width) );
    }
}

void Ui::drawProcStat(Point start, proc_data* news) const {
    char* buff = new char[w_width-1];
    snprintf(buff, w_width-1,
            "%6i  %25s  %4i  %4i  %10i  %10i  %10i  %8i %c  %40s\0",
            news->pid, news->user.c_str(), news->pri, news->ni, news->virt,
            news->res, news->shr, news->ltime, news->state, news->name.c_str());
    std::string to_render(buff);
    drawString(start, to_render);
    delete buff;
}

void Ui::drawProcList(Point start, std::vector<proc_data>& pnews) const {
    int i = 0;
    for (auto it = pnews.begin()+Ui::firstToDraw; it != pnews.end(); it++) {
        drawProcStat(Point(start.x_, start.y_+i), &(*it));
        i++;
    }
}

void Ui::drawAll(Point start, data& news, std::vector<proc_data>& pnews) const {
    /*std::cerr << news.uptime << '\n'
        << news.idle << '\n'
        << news.load[0] << " " << news.load[1] << " " << news.load[2] << '\n'
        << news.usage[0] << " " << news.usage[1] << " " << news.usage[2] << " " << news.usage[3] << '\n'
        << news.threads << '\n'
        << news.running << '\n';*/

    tb_clear();
    drawStats(start, news.usage);
    drawSummary(Point(w_width/2+5, start.y_),
            news.load, news.threads, news.running, news.uptime);
    drawSeparator(start.y_+5);
    drawProcList(Point(start.x_, start.y_+6), pnews);
    tb_present();
}

void Ui::ui_loop(data& news) {
    set_width();
    set_height();
    //drawAll(Point(1,2), news);

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
                set_height();
                set_width();
                //drawAll(Point(1,2), news);
                break;
        }
    }
done:
    return;
}
