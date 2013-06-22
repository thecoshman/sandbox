#pragma once
#include <vector>
#include <string>

namespace Pixel{
    enum class Format{
        rgba8
    };

    struct rgba8{
        unsigned char r,g,b,a;
    };
}

struct imageData{
    long int width, height;
    Pixel::Format format;
    std::vector<Pixel::rgba8> data;
};

imageData loadImage(const std::string& imagePath);