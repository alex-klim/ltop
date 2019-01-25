#include "include/ui.hpp"

#include <iostream>


int Ui::w_height = 0;
int Ui::w_width = 0;

void Ui::set_height() {
    Ui::w_height = tb_height();
}

void Ui::set_width() {
    Ui::w_width = tb_width();
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

void Ui::drawCpuLoad(int y, int startx, int maxx, int load) const {
    tb_change_cell(startx, y, '[', 151, 236);
    for (auto i = startx+1; i < load; i++) {
        tb_change_cell(i, y, '=', 151, 236);
    }
    tb_change_cell(maxx, y, ']', 151, 236);
}

void Ui::drawSeparator(int y) const {
    for (auto i = 0; i < Ui::w_width; ++i) {
        tb_change_cell(i, y, ' ', 151, 151);
    }
}

void Ui::drawString(int startx, int starty, std::string& msg) const {
    for (size_t i = 0; i < msg.length(); i++) {
        tb_change_cell(startx+i, starty, msg[i], 151, 236);
    }
}

void Ui::drawSummary(int startx, int starty, data& d) const {
    std::string buf = "Tasks: " + std::to_string(d.threads)
        + ", " + std::to_string(d.running) +" running";
    drawString(startx, starty, buf);
    buf = "Load average: " + std::to_string(d.load[0]) + ", " + std::to_string(d.load[1])
        + ", " + std::to_string(d.load[2]);
    drawString(startx, starty+1, buf);
    buf = "Uptime: " + std::to_string(d.uptime);
    drawString(startx, starty+2, buf);
}

void Ui::drawHeader(data& news) const {
    double arr[4];
    for (auto i = 0; i < 4; ++i) {
        arr[i] = i + i*30;
    }
    int startx = 2;
    int starty = 1;
    int max_width = Ui::w_width/2;

    for (auto i = 0; i < 4; i++) {
        drawCpuLoad(starty+i, startx, max_width, (int)(arr[i]/100*max_width) );
    }
    drawSeparator(starty+5);

    drawSummary(max_width+5, 1, news);
}

void Ui::drawAll(data& news) const {
    tb_clear();
    drawHeader(news);
    tb_present();
}

void Ui::ui_loop(data& news) const {
    Ui::set_width();
    Ui::set_height();
    drawAll(news);

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
                drawAll(news);
                break;
        }
    }
done:
    return;
}
