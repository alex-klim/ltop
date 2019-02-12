#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cstring>

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

