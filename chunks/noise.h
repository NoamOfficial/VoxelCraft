#pragma once

#include <cmath>
#include <cstdint>

class Noise3D {
public:
    explicit Noise3D(uint32_t seed)
        : seed(seed) {}

    float sample(float x, float y, float z) const
    {
        int x0 = static_cast<int>(std::floor(x));
        int y0 = static_cast<int>(std::floor(y));
        int z0 = static_cast<int>(std::floor(z));

        float xf = x - x0;
        float yf = y - y0;
        float zf = z - z0;

        float u = fade(xf);
        float v = fade(yf);
        float w = fade(zf);

        float n000 = random(x0,     y0,     z0);
        float n100 = random(x0 + 1, y0,     z0);
        float n010 = random(x0,     y0 + 1, z0);
        float n110 = random(x0 + 1, y0 + 1, z0);

        float n001 = random(x0,     y0,     z0 + 1);
        float n101 = random(x0 + 1, y0,     z0 + 1);
        float n011 = random(x0,     y0 + 1, z0 + 1);
        float n111 = random(x0 + 1, y0 + 1, z0 + 1);

        float x00 = lerp(n000, n100, u);
        float x10 = lerp(n010, n110, u);
        float x01 = lerp(n001, n101, u);
        float x11 = lerp(n011, n111, u);

        float y0v = lerp(x00, x10, v);
        float y1v = lerp(x01, x11, v);

        return lerp(y0v, y1v, w);
    }

private:
    uint32_t seed;

    static float fade(float t)
    {
        return t * t * (3.0f - 2.0f * t);
    }

    static float lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    float random(int x, int y, int z) const
    {
        uint32_t h = seed;

        h ^= static_cast<uint32_t>(x) * 374761393u;
        h ^= static_cast<uint32_t>(y) * 668265263u;
        h ^= static_cast<uint32_t>(z) * 2147483647u;

        h ^= h >> 13;
        h *= 1274126177u;
        h ^= h >> 16;

        return (h / 4294967295.0f) * 2.0f - 1.0f;
    }
};
