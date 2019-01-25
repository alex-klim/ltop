#include "include/client.hpp"
#include <iostream>
#include <chrono>
#include <thread>

void cpu_cpy(cpu_info* lhs, cpu_info* rhs) {
    for (auto i = 0; i < 9; ++i) {
        lhs[i] = rhs[i];
    }
}

void calc_usage(cpu_info* lhs, cpu_info* rhs, double* usage) {
    ull all;
    ull work;
    for (auto i = 1; i < 5; ++i) {
        ull all = (rhs[i].user + rhs[i].system + rhs[i].nice + rhs[i].idle + rhs[i].iowait + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice + lhs[i].idle + lhs[i].iowait + lhs[i].irq + lhs[i].sirq);
        ull work = (rhs[i].user + rhs[i].system + rhs[i].nice + rhs[i].irq + rhs[i].sirq)
            - (lhs[i].user + lhs[i].system + lhs[i].nice + lhs[i].irq + lhs[i].sirq);
        usage[i-1] = static_cast<double>(work)/all;
    }
}

int main() {
    Client client;
    double upt, idt, av1, av5, av15;
    ull runnable, exists, memtot, memfree, memav, stot, sfree;
    cpu_info* arr = new cpu_info[9];
    cpu_info* last = new cpu_info[9];
    double usage[4];

    client.uptime(upt, idt);
    client.loadavg(av1, av5, av15, runnable, exists);
    client.meminfo(memtot, memfree, memav, stot, sfree);
    client.stat(arr);

    std::cout << "Lets begin out tests. Hooray!\n"
              << "First of all lets examine uptime:\n"
              << "Uptime: " << upt
              << "\nIddle time: " << idt << '\n';

    std::cout << "=====================================\n"
              << "Now loadavg time:\n"
              << "av1: " << av1
              << "\nav5: " << av5
              << "\nav15: " << av15
              << "\nrunnable scheduling entities: " << runnable <<'\n';
    std::cout << "=====================================\n"
              << "Now meminfo:\n"
              << "MemTotal: " << memtot
              << "\nMemFree: " << memfree
              << "\nMemAvailable: " << memav
              << "\nSwapTotal: " << stot
              << "\nSwapFree:" << sfree <<'\n';

    while (1) {
        cpu_cpy(last, arr);
        client.stat(arr);
        calc_usage(last, arr, usage);

        std::cout << "=====================================================================\n"
              << "Now cpus:\n"
              << "user: " << arr[0].user
              << "\nnice: " << arr[0].nice
              << "\nsystem: " << arr[0].system
              << "\nidle: " << arr[0].idle
              << "\niowait:" << arr[0].iowait <<'\n';
        std::cout << "=====================================\n"
              << "Now cpu0:\n"
              << "user: " << arr[1].user
              << "\nnice: " << arr[1].nice
              << "\nsystem: " << arr[1].system
              << "\nidle: " << arr[1].idle
              << "\niowait:" << arr[1].iowait <<'\n'
              << "Cpu0 usage: " << usage[0] << '\n';
        std::cout << "=====================================\n"
              << "Now cpu1:\n"
              << "user: " << arr[2].user
              << "\nnice: " << arr[2].nice
              << "\nsystem: " << arr[2].system
              << "\nidle: " << arr[2].idle
              << "\niowait:" << arr[2].iowait <<'\n'
              << "Cpu1 usage: " << usage[1] << '\n';
        std::cout << "=====================================\n"
              << "Now cpu2:\n"
              << "user: " << arr[3].user
              << "\nnice: " << arr[3].nice
              << "\nsystem: " << arr[3].system
              << "\nidle: " << arr[3].idle
              << "\niowait:" << arr[3].iowait <<'\n'
              << "Cpu2 usage: " << usage[2] << '\n';
        std::cout << "=====================================\n"
              << "Now cpu3:\n"
              << "user: " << arr[4].user
              << "\nnice: " << arr[4].nice
              << "\nsystem: " << arr[4].system
              << "\nidle: " << arr[4].idle
              << "\niowait:" << arr[4].iowait <<'\n'
              << "Cpu3 usage: " << usage[3] << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(Client::timeDelta));
    }

    delete[] arr;
    delete[] last;
    return 0;
}
