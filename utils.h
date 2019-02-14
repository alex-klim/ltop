#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cstring>
#include <array>

#include "containers.hpp"

inline void cpu_cpy(std::array<cpu_info, 9>& dst, std::array<cpu_info, 9>& src) {
    for (auto i = 0; i < 9; ++i) {
        dst[i] = src[i];
    }
}

inline void calc_usage(std::array<cpu_info, 9>& lhs, std::array<cpu_info, 9>& rhs, double usage[4]) {
    ull all;
    ull work;
    for (auto i = 1; i < 5; ++i) {
        ull all = (rhs[i].user + rhs[i].system + rhs[i].nice
                + rhs[i].idle + rhs[i].iowait + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice
                + lhs[i].idle + lhs[i].iowait + lhs[i].irq + lhs[i].sirq);
        ull work = (rhs[i].user + rhs[i].system + rhs[i].nice + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice + lhs[i].irq + lhs[i].sirq);
        usage[i-1] = static_cast<double>(work)/all;
    }
}

inline bool is_num(std::string& p) {
    for (auto& ch: p) {
        if (!std::isdigit(ch))
            return 0;
    }
    return 1;
}

inline void ttcs(char* dst, unsigned long long time) {
    unsigned long hour, minute, sec;
    hour = time/(60*60);
    time -= hour*60*60;
    minute = time/60;
    time -= minute*60;
    sec = time;
    sprintf(dst, "%0*lu:%0*lu:%0*lu", 2, hour, 2, minute, 2, sec);
}

inline std::string tts(unsigned long time) {
    std::string tmp;
    unsigned long hour, minute, sec;
    hour = time/(60*60);
    time -= hour*60*60;
    minute = time/60;
    time -= minute*60;
    sec = time;
    tmp = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(sec);
    return tmp;
}

#endif // UTILS_HPP

