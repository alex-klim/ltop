#include "include/client.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


static std::string STAT_FILE("/proc/stat");
static std::string UPTIME_FILE("/proc/uptime");
static std::string LOADAVG_FILE("/proc/loadavg");
static std::string MEMINFO_FILE("/proc/meminfo");

constexpr int Client::timeDelta;

void Client::uptime(ull& upt, ull& idt) const {
    std::ifstream ifs(UPTIME_FILE);

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file\n";
        return;
    }
    double tmp;
    ifs >> tmp;
    upt = (unsigned long long)(tmp);
    ifs >> tmp;
    idt = (unsigned long long)(tmp);

}

void Client::loadavg(double& avg1, double& avg5, double& avg15, int& run, int& threads) const {
    std::ifstream ifs(LOADAVG_FILE);

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file\n";
        return;
    }

    ifs >> avg1;
    ifs >> avg5;
    ifs >> avg15;
    ifs >> run;
    ifs >> threads;

}

void Client::stat(cpu_info* cpus) const {
    std::ifstream ifs(STAT_FILE);
    std::string line;
    int i = 0;

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file\n";
        return;
    }

    while (std::getline(ifs, line)) {
        // not sure if that's portable
        if (line.find("cpu") != std::string::npos) {
            std::stringstream ss(line);
            ss.seekg(4);
            ss >> cpus[i].user;
            ss >> cpus[i].nice;
            ss >> cpus[i].system;
            ss >> cpus[i].idle;
            ss >> cpus[i].iowait;
            ss >> cpus[i].irq;
            ss >> cpus[i].sirq;
        }
        i++;
    }
}

void Client::meminfo(ull& memtot, ull& memfree, ull& memav,
        ull& swptot, ull& swpfree) const {
    std::ifstream ifs(MEMINFO_FILE);
    std::string line;

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file\n";
        return;
    }

    for (auto i = 0; std::getline(ifs, line); ++i) {
        //std::cout << "Line" << i << ": " << line << '\n';
        switch(i) {
            case 0: {
                std::stringstream temp;
                std::string garbage;
                temp << line;
                temp >> garbage;
                temp >> memtot;
                break;
                }
            case 1: {
                std::stringstream temp;
                std::string garbage;
                temp << line;
                temp >> garbage;
                temp >> memfree;
                break;
                }
            case 2: {
                std::stringstream temp;
                std::string garbage;
                temp << line;
                temp >> garbage;
                temp >> memav;
                break;
                }
            case 14: {
                std::stringstream temp;
                std::string garbage;
                temp << line;
                temp >> garbage;
                temp >> swptot;
                break;
                }
            case 15: {
                std::stringstream temp;
                std::string garbage;
                temp << line;
                temp >> garbage;
                temp >> swpfree;
                break;
                }
        }
        line.clear();
    }

}
