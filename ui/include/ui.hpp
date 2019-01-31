#ifndef UI_CPP
#define UI_CPP

#include <termbox.h>

#include <string>

using ull = unsigned long long;

struct data {
    ull uptime;
    ull idle;
    double load[3];
    double usage[4];
    int threads;
    int running;
};

struct Point {
    Point() : x_(0), y_(0){};
    Point(int x, int y) : x_(x), y_(y){};
    Point(const Point& p) : x_(p.x_), y_(p.y_){};

    int x_, y_;
};

class Ui {
public:
    Ui() {};

    void init() const;
    void on_exit() const;
    void drawStats(Point, double[4]) const;
    void drawSeparator(int) const;
    void drawProcList(int, int) const;
    void drawCpuLoad(Point, int, int) const;
    void drawSummary(Point, double[3], int, int, ull) const;
    void drawString(Point, std::string&) const;
    void drawAll(Point, data&) const;
    void ui_loop(data&);

    void set_width();
    void set_height();
private:
    int w_height;
    int w_width;
};


#endif
