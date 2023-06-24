#pragma once
#include "utils.hpp"
#include <tuple>
#include <vector>

int sampleJuliaIteration(ComplexDouble_t z, ComplexDouble_t c);

std::vector<float> getJuliaLuminanceMap(ComplexDouble_t c, size_t imgWidth, size_t imgHeight,
                                        std::pair<double, double> xRange, std::pair<double, double> yRange);
