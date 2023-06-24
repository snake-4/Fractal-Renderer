#include <SFML/Graphics.hpp>
#include <cstdint>
#include <tuple>
#include <cmath>
#include <chrono>
#include <execution>
#include <iostream>
#include "utils.hpp"
#include "julia.hpp"
#include "rgbUtils.hpp"

using namespace std::chrono;
std::vector<uint8_t> getJuliaBitmapRGBA(ComplexDouble_t c, XYPairDouble_t imgSize, XYPairDouble_t minRange, XYPairDouble_t maxRange);
void centerAndScaleSprite(sf::Sprite &sprite, sf::Vector2u windowSize);

int main()
{
    constexpr int imgWidth = 400;
    constexpr int imgHeight = 400;
    constexpr double zoomFactor = 1.5;
    constexpr ComplexDouble_t c(0, 0.8);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Render Window");

    double zoomLevel = 1.0;
    XYPairDouble_t panOffset = {0.0, 0.0};

    auto calculateRenderXYRanges = [&]() -> std::pair<XYPairDouble_t, XYPairDouble_t>
    {
        return {
            {-1.0 * zoomLevel + panOffset.x, -1.0 * zoomLevel + panOffset.y},
            {1.0 * zoomLevel + panOffset.x, 1.0 * zoomLevel + panOffset.y}};
    };

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Vector2i lastMousePos;
    bool isDragging = false;
    bool requiresRedraw = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Resized:
            {
                requiresRedraw = true;
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    zoomLevel *= (event.mouseWheelScroll.delta < 0 ? zoomFactor : 1 / zoomFactor);
                    requiresRedraw = true;
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (isDragging)
                {
                    sf::Vector2i delta = sf::Vector2i(event.mouseMove.x, event.mouseMove.y) - lastMousePos;

                    sf::Vector2u windowSize = window.getSize();
                    double delta_x = (double)delta.x / windowSize.x;
                    double delta_y = (double)delta.y / windowSize.y;

                    auto [rangeMin, rangeMax] = calculateRenderXYRanges();

                    panOffset.x += -1 * (delta_x) * (rangeMax.x - rangeMin.x);
                    panOffset.y += (delta_y) * (rangeMax.y - rangeMin.y);
                    requiresRedraw = true;
                }
                lastMousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    isDragging = true;
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    isDragging = false;
                break;
            }
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            default:
                break;
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (requiresRedraw)
        {
            window.clear();

            auto [rangeMin, rangeMax] = calculateRenderXYRanges();
            std::cout << "Range of x: " << rangeMax.x - rangeMin.x << '\n';

            std::vector<uint8_t> imageBitmap = getJuliaBitmapRGBA(c, {imgWidth, imgHeight}, rangeMin, rangeMax);

            image.create(imgWidth, imgHeight, imageBitmap.data());
            texture.loadFromImage(image);
            sprite.setTexture(texture, true);

            centerAndScaleSprite(sprite, window.getSize());

            window.draw(sprite);
            window.display();

            requiresRedraw = false;
        }
    }
}

void centerAndScaleSprite(sf::Sprite &sprite, sf::Vector2u windowSize)
{
    auto localBounds = sprite.getLocalBounds();
    auto scaleFactor = std::min(windowSize.x / localBounds.width, windowSize.y / localBounds.height);

    sprite.setScale(scaleFactor, scaleFactor);
    // sprite.setPosition((windowSize.x / 2.f) - localBounds.width, 0);
}

std::vector<uint8_t> getJuliaBitmapRGBA(ComplexDouble_t c, XYPairDouble_t imgSize, XYPairDouble_t minRange, XYPairDouble_t maxRange)
{
    constexpr size_t bytesPerPixel = 4;

    auto t1Start = high_resolution_clock::now();
    std::vector<float> luminanceMap = getJuliaLuminanceMap(c, imgSize, minRange, maxRange);
    float *luminanceMapData = luminanceMap.data();
    auto t1Diff = duration_cast<milliseconds>(high_resolution_clock::now() - t1Start);
    std::cout << "Calculating the julia set took " << t1Diff.count() << "ms.\n";

    auto t2Start = high_resolution_clock::now();
    std::vector<uint8_t> imageBitmap(static_cast<size_t>(imgSize.x * imgSize.y * bytesPerPixel));
    std::for_each(
        std::execution::par_unseq,
        luminanceMapData, luminanceMapData + luminanceMap.size(),
        [=, &imageBitmap](const float &item)
        {
            auto idx = &item - luminanceMapData;

            // MAGIC NUMBERS AHEAD
            int hsv_h = static_cast<int>(mapValue(item * 360.0, {0.0, 360.0}, {170.0, 260.0}));
            int hsv_v = static_cast<int>(item * 100 * 4);
            if (hsv_h > 360)
                hsv_h -= 360;
            if (hsv_v > 100)
                hsv_v = 100;

            auto hsv = std::make_tuple(hsv_h, 100, hsv_v);
            auto [r, g, b] = mapHSVToRGB(hsv);

            size_t rgbaIndex = idx * bytesPerPixel;
            imageBitmap[rgbaIndex] = r;
            imageBitmap[++rgbaIndex] = g;
            imageBitmap[++rgbaIndex] = b;
            imageBitmap[++rgbaIndex] = 255; // Alpha
        });
    auto t2Diff = duration_cast<milliseconds>(high_resolution_clock::now() - t2Start);
    std::cout << "Converting luminance map to RGB took " << t2Diff.count() << "ms.\n";

    return imageBitmap;
}
