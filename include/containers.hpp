#ifndef CONT_HPP
#define CONT_HPP

#include <vector>
#include <memory>

using ull = unsigned long long;

struct cpu_info {
    ull user, nice, system, idle, iowait, irq, sirq;
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
