#include "include/client.hpp"

#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <pwd.h>

static std::string STAT_FILE("/proc/stat");
static std::string UPTIME_FILE("/proc/uptime");
static std::string LOADAVG_FILE("/proc/loadavg");
static std::string MEMINFO_FILE("/proc/meminfo");

// ============ carefully borrowed from procps =============
#define P_G_SZ 20
#define	HASHSIZE	64		/* power of 2 */
#define	HASH(x)		((x) & (HASHSIZE - 1))

static struct pwbuf {
    struct pwbuf *next;
    uid_t uid;
    char name[P_G_SZ];
} *pwhash[HASHSIZE];

char *user_from_uid(uid_t uid) {
    struct pwbuf **p;
    struct passwd *pw;

    p = &pwhash[HASH(uid)];
    while (*p) {
	if ((*p)->uid == uid)
	    return((*p)->name);
	p = &(*p)->next;
    }
    *p = (struct pwbuf *) malloc(sizeof(struct pwbuf));
    (*p)->uid = uid;
    pw = getpwuid(uid);
    if(!pw || strlen(pw->pw_name) >= P_G_SZ)
	sprintf((*p)->name, "%u", uid);
    else
        strcpy((*p)->name, pw->pw_name);

    (*p)->next = NULL;
    return((*p)->name);
}
// ============== end of borrowed content ===================

void Client::uptime(ull& upt, ull& idt) const {
    std::ifstream ifs(UPTIME_FILE);

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file: uptime\n";
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
    char slash;

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file: loadavg\n";
        return;
    }

    ifs >> avg1;
    ifs >> avg5;
    ifs >> avg15;
    ifs >> run;
    ifs >> slash;
    ifs >> threads;

}

void Client::stat(std::array<cpu_info, 9>& cpus) const {
    std::ifstream ifs(STAT_FILE);
    std::string line;
    int i = 0;

    if(!ifs.is_open()) {
        std::cerr << "Failed to read file: stat\n";
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
        std::cerr << "Failed to read file: meminfo\n";
        return;
    }

    for (auto i = 0; std::getline(ifs, line); ++i) {
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

void Client::procstat(std::string filename, proc_data* data) const {
    std::ifstream ifs(filename);
    std::string line;

    #ifdef DEBUG
    if(!ifs.is_open()) {
        std::cerr << "Failed to read file: procstat\n";
        return;
    }
    #endif

    for (auto i = 1; i < 25; i++) {
        ifs >> line;
        switch (i) {
            case 1: {
                    std::stringstream temp;
                    temp << line;
                    temp >> data->pid;
                    break;
                    }
                    // trim braces from process name
            case 2: data->name = std::string(line.cbegin()+1, line.cend()-1); break;
            case 3: {
                    std::stringstream temp;
                    temp << line;
                    temp >> data->state;
                    break;
                    }
            case 14: {
                    std::stringstream temp;
                    temp << line;
                    temp >> data->ltime;
                    break;
                     }
            case 15: {
                    std::stringstream temp;
                    int tmp;
                    temp << line;
                    temp >> tmp;
                    data->ltime += tmp;
                    break;
                    }
            case 18: {
                    std::stringstream temp;
                    temp << line;
                    temp >> data->pri;
                    break;
                     }
            case 19: {
                    std::stringstream temp;
                    temp << line;
                    temp >> data->ni;
                    break;
                     }
            case 23: {
                    std::stringstream temp;
                    temp << line;
                    temp >> data->virt;
                    data->virt /= 1024;
                    break;
                     }
            case 24: {
                    std::stringstream temp;
                    temp << line;
                    temp >> data->res;
                    data->res *= 4;
                    break;
                     }
        }
        line.clear();
    }
}

void Client::procstatus(std::string filename, proc_data* data) const {
    std::ifstream ifs(filename);
    std::string garbage;

    #ifdef DEBUG
    if(!ifs.is_open()){
        std::cerr << "couldn't open the file: procstatus\n";
        return;
    }
    #endif

    for (auto i = 0; i < 9; i++) {
        garbage.clear();
        std::getline(ifs, garbage);
    }
    std::stringstream tmp(garbage);
    garbage.clear();
    tmp >> garbage;
    int uid;
    tmp >> uid;
    data->user = user_from_uid(uid);
}
