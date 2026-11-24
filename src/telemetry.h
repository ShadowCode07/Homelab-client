#pragma once

#include <cstdint>
#include <optional>
#include <string>

struct CpuTimes {
    std::uint64_t idle = 0;
    std::uint64_t total = 0;
};

struct MemInfo {
    std::uint64_t totalBytes = 0;
    std::uint64_t availableBytes = 0;
};

struct DiskInfo {
    std::uint64_t totalBytes = 0;
    std::uint64_t availableBytes = 0;
};

std::optional<CpuTimes> readCpuTimes();
std::optional<MemInfo> readMemInfo();

std::optional<std::uint64_t> readUptimeSeconds();

double cpuPercent(const CpuTimes& prev, const CpuTimes& curr);

std::optional<DiskInfo> readDiskInfo(const std::string& path);



