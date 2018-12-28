#include "include/client.hpp"
#include <iostream>

int main() {
    Client client;
    double upt, idt, av1, av5, av15;
    ull runnable, memtot, memfree, memav, stot, sfree;
    cpu_info* arr = new cpu_info[9];

    client.uptime(upt, idt);
    client.loadavg(av1, av5, av15, runnable);
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
    std::cout << "=====================================\n"
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
              << "\niowait:" << arr[1].iowait <<'\n';
    std::cout << "=====================================\n"
              << "Now cpu1:\n"
              << "user: " << arr[2].user
              << "\nnice: " << arr[2].nice
              << "\nsystem: " << arr[2].system
              << "\nidle: " << arr[2].idle
              << "\niowait:" << arr[2].iowait <<'\n';
    std::cout << "=====================================\n"
              << "Now cpu2:\n"
              << "user: " << arr[3].user
              << "\nnice: " << arr[3].nice
              << "\nsystem: " << arr[3].system
              << "\nidle: " << arr[3].idle
              << "\niowait:" << arr[3].iowait <<'\n';
    std::cout << "=====================================\n"
              << "Now cpu3:\n"
              << "user: " << arr[4].user
              << "\nnice: " << arr[4].nice
              << "\nsystem: " << arr[4].system
              << "\nidle: " << arr[4].idle
              << "\niowait:" << arr[4].iowait <<'\n';

    delete[] arr;
    return 0;
}
