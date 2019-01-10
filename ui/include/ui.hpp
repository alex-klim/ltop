#ifndef UI_CPP
#define UI_CPP

#include <termbox.h>

#include <string>

struct data {
    int threads;
    int running;
    double load[3];
    unsigned long long uptime;
};

class Ui {
public:
    Ui() {};

    void init() const;
    void drawHeader(data&) const;
    void drawSeparator(int) const;
    void drawProcList(int, int) const;
    void drawCpuLoad(int, int, int, int) const;
    void drawSummary(int, int, data&) const;
    void drawString(int, int, std::string&) const;
    void ui_loop(data&) const;

    static int w_height;
    static int w_width;

    static int get_height();
    static int get_weight();
private:
    static void set_height();
    static void set_width();
};


#endif
