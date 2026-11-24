#include "telemetry.h"

#include <fstream>
#include <sstream>
#include <string>

std::optional<CpuTimes> readCpuTimes() {
    std::ifstream cpuTimesFile("/proc/stat");

    if (!cpuTimesFile.is_open())
        return std::nullopt;

    std::string cpuTimes;
    std::getline(cpuTimesFile, cpuTimes);

    std::istringstream in(cpuTimes);
    std::string name;
    std::uint64_t user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0;
    in >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    if (name != "cpu")
        return std::nullopt;

    std::uint64_t idleTime = idle + iowait;
    std::uint64_t totalTime = user + nice + system +idle + iowait + irq + softirq + steal;

    CpuTimes readings {idleTime, totalTime};
    return readings;
}

std::optional<MemInfo> readMemInfo() {
    std::ifstream memInfoFile("/proc/meminfo");

    if (!memInfoFile.is_open())
        return std::nullopt;

    std::string memLine;
    bool totalFlag = false, availableFlag = false;
    std::uint64_t totalBytes = 0, availableBytes = 0;

    while (std::getline(memInfoFile, memLine)) {
        std::istringstream in(memLine);
        std::string name;
        std::uint64_t kbytes = 0;
        in >> name >> kbytes;
        if (name == "MemTotal:") {
            totalBytes = kbytes * 1024;
            totalFlag = true;
        }
        else if (name == "MemAvailable:") {
            availableBytes = kbytes * 1024;
            availableFlag = true;
        }

        if (totalFlag && availableFlag)
            break;
    }

    if (!totalFlag || !availableFlag)
        return std::nullopt;

    MemInfo memInfo {totalBytes, availableBytes};
    return memInfo;
}

std::optional<std::uint64_t> readUptimeSeconds() {
    std::ifstream uptimeFile("/proc/uptime");

    if (!uptimeFile.is_open())
        return std::nullopt;

    std::string line;
    std::getline(uptimeFile, line);

    std::istringstream in(line);
    double uptime = 0;
    in >> uptime;

    if (!in)
        return std::nullopt;

    return static_cast<std::uint64_t>(uptime);
}