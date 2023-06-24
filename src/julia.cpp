#include <vector>
#include <cmath>
#include <complex>
#include <cstddef>
#include <tuple>
#include <execution>
#include "utils.hpp"
#include "julia.hpp"

int sampleJuliaIteration(ComplexDouble_t z, ComplexDouble_t c)
{
    constexpr size_t maxIter = 500;
    constexpr size_t maxMagnitude = 2;

    for (int i = 1; i <= maxIter; i++)
    {
        z = std::pow(z, 2) + c;
        if (std::abs(z) >= maxMagnitude)
        {
            return i;
        }
    }
    return 0;
}

std::vector<float> getJuliaLuminanceMap(ComplexDouble_t c, size_t imgWidth, size_t imgHeight,
                                        std::pair<double, double> xRange, std::pair<double, double> yRange)
{
    /*
     * Because we start drawing from top left:
     * Real part(x axis) starts from minimum.
     * Imaginary part(y axis) starts from maximum.
     * So we swap the y axis' minimum and maximum
     */
    std::swap(yRange.first, yRange.second);

    std::vector<float> luminanceMap(imgHeight * imgWidth);
    float *rawData = luminanceMap.data();

    std::for_each(
        std::execution::par_unseq,
        rawData, rawData + luminanceMap.size(),
        [=](float &item)
        {
            auto idx = &item - rawData;
            auto y = idx / imgWidth;
            auto x = idx % imgWidth;

            double realPart = mapValue(static_cast<double>(x), {0, static_cast<double>(imgWidth)}, xRange);
            double imagPart = mapValue(static_cast<double>(y), {0, static_cast<double>(imgHeight)}, yRange);
            ComplexDouble_t z(realPart, imagPart);

            // These luminance values are unnormalized!
            item = static_cast<float>(sampleJuliaIteration(z, c));
        });

    // Normalize max to 1
    auto maxLum = *std::max_element(luminanceMap.begin(), luminanceMap.end());
    for (auto &val : luminanceMap)
    {
        val /= maxLum;
    }

    return luminanceMap;
}
