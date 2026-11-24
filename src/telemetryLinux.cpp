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
