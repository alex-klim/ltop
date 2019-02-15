#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <array>
#include "containers.hpp"

class Client {
public:

    Client() = default;
    void uptime(ull&, ull&) const;
    void loadavg(double&, double&, double&, int&, int&) const;
    void stat(std::array<cpu_info, 9>&) const;
    void meminfo(ull&, ull&, ull&, ull&, ull&) const;
    void procstat(std::string, proc_data*) const;
    void procstatus(std::string, proc_data*) const;
};

#endif
