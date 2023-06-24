#pragma once
#include <complex>
#include <initializer_list>
#include <tuple>

using ComplexDouble_t = std::complex<double>;

struct XYPairDouble_t
{
    XYPairDouble_t();
    XYPairDouble_t(double x, double y);
    double x = 0;
    double y = 0;
};

inline double mapValue(double value, std::pair<double, double> inRange, std::pair<double, double> outRange)
{
    const auto &inMin = inRange.first;
    const auto &inMax = inRange.second;
    const auto &outMin = outRange.first;
    const auto &outMax = outRange.second;
    return ((value - inMin) / (inMax - inMin)) * (outMax - outMin) + outMin;
};
