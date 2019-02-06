#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

bool is_num(std::string& p) {
    for (auto& ch: p) {
        if (!std::isdigit(ch))
            return 0;
    }
    return 1;
}

#endif // UTILS_HPP

