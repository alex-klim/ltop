#ifndef UI_CPP
#define UI_CPP

#include <termbox.h>

#include <string>

class Ui {
public:
    Ui() {};

    void init() const;
    void drawHeader() const;
    void drawSeparator(int) const;
    void drawProcList(int, int) const;
    void drawCpuLoad(int, int, int, int) const;
    void drawSummary(int, int, int, int, double*, unsigned long long) const;
    void drawString(int, int, std::string&) const;
    void ui_loop() const;

    static int w_height;
    static int w_width;

    static int get_height();
    static int get_weight();
private:
    static void set_height();
    static void set_width();
};


#endif
