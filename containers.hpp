#ifndef CONT_HPP
#define CONT_HPP

#include <vector>
#include <memory>

struct proc_data {
    double pcpu, pmem;
    int pid;
    std::string user, name;
    int pri, ni, virt, res, shr, ltime;
    char state;

    //proc_data(int a) : pid(a){};
};

using ull = unsigned long long;

struct minfo {
    ull memtot;
    ull memfree;
    ull memav;
    ull stot;
    ull sfree;
};

#endif // CONT_HPP
