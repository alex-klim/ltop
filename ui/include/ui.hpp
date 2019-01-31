#ifndef UI_CPP
#define UI_CPP

#include <termbox.h>

#include <string>

using ull = unsigned long long;

using data = struct {
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
    void drawStats(Point, double[4]) const;
    void drawSeparator(int) const;
    void drawProcList(int, int) const;
    void drawCpuLoad(Point, int, int) const;
    void drawSummary(Point, double[3], int, int, ull) const;
    void drawString(Point, std::string&) const;
    void drawAll(Point, data&) const;
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
