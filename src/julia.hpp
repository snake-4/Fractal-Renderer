#pragma once
#include "utils.hpp"
#include <tuple>
#include <vector>

int sampleJuliaIteration(ComplexDouble_t z, ComplexDouble_t c);

std::vector<float> getJuliaLuminanceMap(ComplexDouble_t c, XYPairDouble_t imageSize,
                                        XYPairDouble_t minRange, XYPairDouble_t maxRange);
