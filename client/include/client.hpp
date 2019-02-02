#ifndef CLIENT_HPP
#define CLIENT_HPP

using ull = unsigned long long;

struct cpu_info {
    ull user, nice, system, idle, iowait, irq, sirq;
};

class Client {
public:
    static constexpr int timeDelta = 3000;

    Client() = default;
    void uptime(ull&, ull&) const;
    void loadavg(double&, double&, double&, int&, int&) const;
    void stat(cpu_info*) const;
    void meminfo(ull&, ull&, ull&, ull&, ull&) const;
};

#endif
