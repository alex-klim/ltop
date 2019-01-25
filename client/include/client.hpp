#ifndef CLIENT_HPP
#define CLIENT_HPP

using ull = unsigned long long;

using cpu_info = struct {
    ull user, nice, system, idle, iowait, irq, sirq;
};

class Client {
public:
    static constexpr int timeDelta = 3000;

    Client() = default;
    void uptime(double&, double&) const;
    void loadavg(double&, double&, double&, ull&, ull&) const;
    void stat(cpu_info*) const;
    void meminfo(ull&, ull&, ull&, ull&, ull&) const;
};

#endif
