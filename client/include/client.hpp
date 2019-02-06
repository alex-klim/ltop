#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "../../containers.hpp"

using ull = unsigned long long;

struct cpu_info {
    ull user, nice, system, idle, iowait, irq, sirq;
};

/*  struct proc_data {
    double cpu, mem;
    int pid;
    std::string name, user;
    int pri, ni, virt, res, shr, ltime;
    char state;
};*/

class Client {
public:
    static constexpr int timeDelta = 3000;

    Client() = default;
    void uptime(ull&, ull&) const;
    void loadavg(double&, double&, double&, int&, int&) const;
    void stat(cpu_info*) const;
    void meminfo(ull&, ull&, ull&, ull&, ull&) const;
    void procstat(std::string, proc_data*) const;
    void procstatus(std::string, std::unique_ptr<proc_data>&) const;
};

#endif
