#include <tuple>
#include <cmath>

std::tuple<int, int, int> mapHSVToRGB(std::tuple<int, int, int> HSV)
{
    auto [h, s, v] = HSV;

    // Convert hue to the range 0-360
    h %= 360;
    if (h < 0) {
        h += 360;
    }

    // Normalize saturation and value to the range 0-1
    float normalizedS = s / 100.0f;
    float normalizedV = v / 100.0f;

    // Convert HSV to RGB
    float c = normalizedV * normalizedS;
    float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
    float m = normalizedV - c;

    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h >= 60 && h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h >= 180 && h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h >= 240 && h < 300) {
        r = x;
        g = 0;
        b = c;
    } else {  // h >= 300 && h < 360
        r = c;
        g = 0;
        b = x;
    }

    // Scale RGB values to the range 0-255
    int red = static_cast<int>((r + m) * 255);
    int green = static_cast<int>((g + m) * 255);
    int blue = static_cast<int>((b + m) * 255);

    return std::make_tuple(red, green, blue);
}

std::tuple<int, int, int> mapHueToRGB(double hue)
{
    hue /= 60.0;
    int i = static_cast<int>(std::floor(hue));
    double f = hue - i;

    double p = 0.0;
    double q = 1.0 - f;
    double t = f;

    int red, green, blue;
    switch (i)
    {
    case 0:
        red = 255;
        green = static_cast<int>(t * 255);
        blue = static_cast<int>(p * 255);
        break;
    case 1:
        red = static_cast<int>(q * 255);
        green = 255;
        blue = static_cast<int>(p * 255);
        break;
    case 2:
        red = static_cast<int>(p * 255);
        green = 255;
        blue = static_cast<int>(t * 255);
        break;
    case 3:
        red = static_cast<int>(p * 255);
        green = static_cast<int>(q * 255);
        blue = 255;
        break;
    case 4:
        red = static_cast<int>(t * 255);
        green = static_cast<int>(p * 255);
        blue = 255;
        break;
    default:
        red = 255;
        green = static_cast<int>(p * 255);
        blue = static_cast<int>(q * 255);
        break;
    }

    return {red, green, blue};
}