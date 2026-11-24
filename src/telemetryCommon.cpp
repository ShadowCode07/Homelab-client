#include <system_error>
#include <filesystem>
#include <iostream>

#include "telemetry.h"

std::optional<double> cpuPercent(const CpuTimes &prev, const CpuTimes &curr) {
    if (prev.idle > curr.idle || prev.total >= curr.total)
        return std::nullopt;

    double percent = (1 - (static_cast<double>(curr.idle - prev.idle) / static_cast<double>(curr.total - prev.total))) * 100;

    return percent;
}

std::optional<DiskInfo> readDiskInfo(const std::string& path) {
    std::error_code ec;
    std::filesystem::space_info info = std::filesystem::space(path, ec);

    if (ec) {
        std::cerr << ec.message() << std::endl;
        return std::nullopt;
    }

    DiskInfo diskInfo {info.capacity, info.available};
    return diskInfo;
}