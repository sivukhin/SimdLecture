#ifndef PROJECT_CPU_CAPABILITIES_HPP
#define PROJECT_CPU_CAPABILITIES_HPP

#include <iostream>
#include <string>

void DescribeCapability(std::string capability, bool has_capability) {
    if (has_capability) {
        std::cout << capability << " support:\tYES" << std::endl;
    } else {
        std::cout << capability << " support:\tNO" << std::endl;
    }
}

#define DescribeCapability(x) DescribeCapability(x, __builtin_cpu_supports(x))

void DescribeCpuCapabilities() {
    DescribeCapability("popcnt");

    DescribeCapability("mmx");
    DescribeCapability("sse");
    DescribeCapability("sse2");
    DescribeCapability("sse3");
    DescribeCapability("ssse3");
    DescribeCapability("sse4.1");
    DescribeCapability("sse4.2");
    DescribeCapability("avx");
    DescribeCapability("avx2");
    DescribeCapability("avx512f");
}

#endif  // PROJECT_CPU_CAPABILITIES_HPP
