#include "include/client.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


static std::string STAT_FILE("/proc/stat");
static std::string UPTIME_FILE("/proc/uptime");
static std::string LOADAVG_FILE("/proc/loadavg");
static std::string MEMINFO_FILE("/proc/meminfo");



void Client::uptime(double& upt, double& idt) const {
    std::ifstream ifs(UPTIME_FILE);

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file\n";
        return;
    }

    ifs >> upt;
    ifs >> idt;

}

void Client::loadavg(double& avg1, double& avg5, double& avg15, ull& run) const {
    std::ifstream ifs(LOADAVG_FILE);

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file\n";
        return;
    }

    ifs >> avg1;
    ifs >> avg5;
    ifs >> avg15;
    ifs >> run;

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
        if (line.find("cpu") != std::string::npos) {
            std::cout << line << '\n';
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
