#include "ui.hpp"
#include "utils.h"

#include <iostream>
#include <cstring>
#include <thread>


static constexpr unsigned int DARK_SEA_GREEN = 151;
static constexpr unsigned int PALE_TURQUOISE = 159;
static constexpr unsigned int GREY_19 = 236;

size_t Ui::firstToDraw = 0;
size_t Ui::currentLine = 0;
size_t Ui::nCpus = std::thread::hardware_concurrency(); // this will be used as a CPU counter

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

void Ui::drawCpuLoad(Point start, int maxx, int load, int cpuN) const {
    tb_change_cell(start.x_, start.y_, '0'+cpuN, DARK_SEA_GREEN, GREY_19);
    tb_change_cell(start.x_+3, start.y_, '[', DARK_SEA_GREEN, GREY_19);
    for (auto i = start.x_+4; i < load; i++) {
        tb_change_cell(i, start.y_, '=', DARK_SEA_GREEN, GREY_19);
    }
    tb_change_cell(maxx, start.y_, ']', DARK_SEA_GREEN, GREY_19);
}

void Ui::drawMemUsage(Point start, int maxx, int load) const {
    std::string label("Mem");
    drawString(start, label);
    tb_change_cell(start.x_+3, start.y_, '[', DARK_SEA_GREEN, GREY_19);
    for (auto i = start.x_+4; i < load; i++) {
        tb_change_cell(i, start.y_, '=', DARK_SEA_GREEN, GREY_19);
    }
    tb_change_cell(maxx, start.y_, ']', DARK_SEA_GREEN, GREY_19);
}

void Ui::drawSwpUsage(Point start, int maxx, int load) const {
    std::string label("Swp");
    drawString(start, label);
    tb_change_cell(start.x_+3, start.y_, '[', DARK_SEA_GREEN, GREY_19);
    for (auto i = start.x_+4; i < load; i++) {
        tb_change_cell(i, start.y_, '=', DARK_SEA_GREEN, GREY_19);
    }
    tb_change_cell(maxx, start.y_, ']', DARK_SEA_GREEN, GREY_19);
}

void Ui::drawSeparator(int y) const {
    char* buff = new char[w_width-1];
    snprintf(buff, w_width-1,
            "%6s  %25s  %4s  %4s  %10s  %10s  %10s  %9s %c  %40s",
            "pid", "User", "PRI", "NI", "VIRT",
            "RES", "SHR", "TIME", 'S', "Process");
    for (size_t i = 0; i < strlen(buff); i++) {
        tb_change_cell(i+1, y, buff[i], GREY_19, DARK_SEA_GREEN);
    }
    for (auto i = strlen(buff)+1; i < w_width; ++i) {
        tb_change_cell(i, y, ' ', DARK_SEA_GREEN, DARK_SEA_GREEN);
    }
    delete[] buff;
}

void Ui::drawString(Point start, std::string& msg) const {
    for (size_t i = 0; i < msg.length(); i++) {
        tb_change_cell(start.x_+i, start.y_, msg[i], DARK_SEA_GREEN, GREY_19);
    }
}

void Ui::drawLine(Point start, std::string& msg) const {
    if (start.y_ == Ui::currentLine - Ui::firstToDraw + Ui::nCpus + 5) {
        for (size_t i = 0; i < msg.length(); i++) {
            tb_change_cell(start.x_+i, start.y_, msg[i], GREY_19, PALE_TURQUOISE);
        }
        for (auto i = msg.length()+1; i < w_width; ++i) {
            tb_change_cell(i, start.y_, ' ', PALE_TURQUOISE, PALE_TURQUOISE);
        }
    } else {
        for (size_t i = 0; i < msg.length(); i++) {
            tb_change_cell(start.x_+i, start.y_, msg[i], DARK_SEA_GREEN, GREY_19);
        }
        for (auto i = msg.length()+1; i < w_width; ++i) {
            tb_change_cell(i, start.y_, ' ', GREY_19, GREY_19);
        }
    }
}

void Ui::drawSummary(Point start, double load[3], int threads, int running, ull uptime) const {
    std::string buf = "Tasks: " + std::to_string(threads)
        + ", " + std::to_string(running) +" running";
    drawString(start, buf);
    buf = "Load average: " + std::to_string(load[0]) + ", " + std::to_string(load[1])
        + ", " + std::to_string(load[2]);
    drawString(Point(start.x_, start.y_+1), buf);
    char cbuf[9];
    ttcs(cbuf, uptime);
    buf = "Uptime: ";
    buf += cbuf;
    drawString(Point(start.x_, start.y_+2), buf);
    buf = "[F9] to send SIGTERM   [Esc] to quit";
    drawString(Point(start.x_, start.y_+3), buf);
}

void Ui::drawStats(Point start, double usage[4], minfo& mi) const {
    int max_width = w_width/2;

    for (size_t i = 0; i < Ui::nCpus; i++) {
        drawCpuLoad(Point(start.x_, start.y_+i), max_width, (int)(usage[i]*(max_width-5)), i+1);
    }
    drawMemUsage(Point(start.x_, start.y_+Ui::nCpus), max_width,
            (int)((double)(mi.memtot-mi.memav)/(double)(mi.memtot)*(max_width-5)));
    drawSwpUsage(Point(start.x_, start.y_+Ui::nCpus+1), max_width,
            (int)((double)(mi.stot-mi.sfree)/(double)(mi.stot)*(max_width-5)));
}

void Ui::drawProcStat(Point start, proc_data* news) const {
    char* buff = new char[w_width-1];
    char timer[9];
    ttcs(timer, news->ltime);
    snprintf(buff, w_width-1,
            "%6i  %25s  %4i  %4i  %10i  %10i  %10i  %9s %c  %40s",
            news->pid, news->user.c_str(), news->pri, news->ni, news->virt,
            news->res, news->shr, timer, news->state, news->name.c_str());
    std::string to_render(buff);
    drawLine(start, to_render);
    delete[] buff;
}

void Ui::drawProcList(Point start, std::vector<proc_data>& pnews) const {
    int i = 0;
    for (auto it = pnews.begin()+Ui::firstToDraw; it != pnews.end(); it++) {
        drawProcStat(Point(start.x_, start.y_+i), &(*it));
        i++;
    }
}

void Ui::drawAll(Point start, data& news, minfo& mi, std::vector<proc_data>& pnews) const {
    tb_clear();
    drawStats(start, news.usage, mi);
    drawSummary(Point(w_width/2+5, start.y_),
            news.load, news.threads, news.running, news.uptime);
    drawSeparator(start.y_+7);
    drawProcList(Point(start.x_, start.y_+8), pnews);
    tb_present();
}
