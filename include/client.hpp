#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <thread>
#include <string>
#include <array>
#include "containers.hpp"

class Client {
public:

    Client() = default;
    void uptime(ull&, ull&) const;
    void loadavg(double&, double&, double&, int&, int&) const;
    void stat(std::array<cpu_info, MAX_CORES_PLUS_ONE>&) const;
    void meminfo(ull&, ull&, ull&, ull&, ull&) const;
    void procstat(std::string, proc_data*) const;
    void procstatus(std::string, proc_data*) const;
};

// ==================== helper functions ==============================
inline void cpu_cpy(std::array<cpu_info, MAX_CORES_PLUS_ONE>& dst,
        std::array<cpu_info, MAX_CORES_PLUS_ONE>& src)
{
    for (auto i = 0; i < MAX_CORES_PLUS_ONE; ++i) {
        dst[i] = src[i];
    }
}

inline void calc_usage(std::array<cpu_info, MAX_CORES_PLUS_ONE>& lhs,
        std::array<cpu_info, MAX_CORES_PLUS_ONE>& rhs,
        double usage[4])
{
    ull all;
    ull work;
    for (auto i = 1; i <= std::thread::hardware_concurrency(); ++i) {
        ull all = (rhs[i].user + rhs[i].system + rhs[i].nice
                + rhs[i].idle + rhs[i].iowait + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice
                + lhs[i].idle + lhs[i].iowait + lhs[i].irq + lhs[i].sirq);
        ull work = (rhs[i].user + rhs[i].system + rhs[i].nice + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice + lhs[i].irq + lhs[i].sirq);
        usage[i-1] = static_cast<double>(work)/all;
    }
}

#endif
