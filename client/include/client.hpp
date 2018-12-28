#ifndef CLIENT_HPP
#define CLIENT_HPP

static constexpr int timeDelta = 1;
using ull = unsigned long long;

struct cpu_info {
    ull user, nice, system, idle, iowait, irq, sirq;
};

class Client {
public:
    Client() = default;
    void uptime(double&, double&) const;
    void loadavg(double&, double&, double&, ull&) const;
    void stat(cpu_info*) const;
    void meminfo(ull&, ull&, ull&, ull&, ull&) const;
};

#endif
