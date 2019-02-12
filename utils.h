#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

inline bool is_num(std::string& p) {
    for (auto& ch: p) {
        if (!std::isdigit(ch))
            return 0;
    }
    return 1;
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

