#ifndef CONT_HPP
#define CONT_HPP

#include <vector>
#include <memory>

using ull = unsigned long long;

static constexpr int MAX_CORES_PLUS_ONE = 9;

struct cpu_info {
    ull user, nice, system, idle, iowait, irq, sirq;
};

struct ginfo {
    ull uptime;
    ull idle;
    double load[3];
    double usage[4];
    int threads;
    int running;
    std::array<cpu_info, MAX_CORES_PLUS_ONE> last;
    std::array<cpu_info, MAX_CORES_PLUS_ONE> cur;
};

struct proc_data {
    double pcpu, pmem;
    int pid;
    std::string user, name;
    int pri, ni, virt, res, shr, ltime;
    char state;
};

struct minfo {
    ull memtot;
    ull memfree;
    ull memav;
    ull stot;
    ull sfree;
};

#endif // CONT_HPP
