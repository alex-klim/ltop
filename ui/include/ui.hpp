#ifndef UI_CPP
#define UI_CPP

#include <termbox.h>

#include <string>
#include "../../containers.hpp"

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
    void drawStats(Point, double[4], minfo&) const;
    void drawSeparator(int) const;
    void drawString(Point, std::string&) const;
    void drawLine(Point, std::string&) const;
    void drawMemUsage(Point, int, int) const;
    void drawSwpUsage(Point, int, int) const;
    void drawProcStat(Point, proc_data*) const;
    void drawProcList(Point, std::vector<proc_data>& ) const;
    void drawCpuLoad(Point, int, int, int) const;
    void drawSummary(Point, double[3], int, int, ull) const;
    void drawAll(Point, data&, minfo&, std::vector<proc_data>&) const;

    void set_width();
    void set_height();
    int get_width() const;
    int get_height() const;
private:
    int w_height;
    int w_width;
public:
    static size_t firstToDraw;
    static size_t currentLine;
    static size_t nCpus;
};


#endif
