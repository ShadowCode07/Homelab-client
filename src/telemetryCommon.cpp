#include "telemetry.h"

std::optional<double> cpuPercent(const CpuTimes &prev, const CpuTimes &curr) {
    if (prev.idle > curr.idle || prev.total >= curr.total)
        return std::nullopt;

    double percent = (1 - (static_cast<double>(curr.idle - prev.idle) / static_cast<double>(curr.total - prev.total))) * 100;

    return percent;
}
