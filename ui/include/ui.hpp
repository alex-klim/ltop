#ifndef UI_CPP
#define UI_CPP

#include <termbox.h>

#include <string>
#include "../../containers.hpp"
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

/*  struct p_data {
    double pcpu, pmem;
    int pid;
    std::string user, name;
    int pri, ni, virt, res, shr, ltime;
    char state;
};*/

class Ui {
public:
    Ui() {};

    void init() const;
    void on_exit() const;
    void drawStats(Point, double[4]) const;
    void drawSeparator(int) const;
    void drawProcStat(Point, proc_data*) const;
    void drawProcList(Point, std::vector<proc_data>& ) const;
    void drawCpuLoad(Point, int, int) const;
    void drawSummary(Point, double[3], int, int, ull) const;
    void drawString(Point, std::string&) const;
    void drawAll(Point, data&, std::vector<proc_data>&) const;
    void ui_loop(data&);

    void set_width();
    void set_height();
    int get_width() const;
    int get_height() const;
private:
    int w_height;
    int w_width;
public:
    static size_t firstToDraw;
    static size_t nCpus;
};


#endif
