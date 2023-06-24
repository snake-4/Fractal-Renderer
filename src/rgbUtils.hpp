#pragma once
#include <tuple>

std::tuple<int, int, int> mapHSVToRGB(std::tuple<int, int, int> HSV);
std::tuple<int, int, int> mapHueToRGB(double hue);
