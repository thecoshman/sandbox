#include "imageLoader.hpp"
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>

namespace Pixel{
    template <typename PixelFormat>
    struct Extractor{
        std::vector<PixelFormat>* storage;
        Extractor(std::vector<PixelFormat>* s) : storage(s) {}
        void operator()(boost::gil::rgba8_pixel_t p) const {
            //get_color(p,red_t())
            //storage->push_back(PixelFormat{boost::gil::at_c<0>(p), boost::gil::at_c<1>(p), boost::gil::at_c<2>(p), boost::gil::at_c<3>(p)});
            //storage->push_back(PixelFormat{
            //    boost::gil::get_color(p, boost::gil::red_t()),
            //    boost::gil::get_color(p, boost::gil::green_t()),
            //    boost::gil::get_color(p, boost::gil::blue_t()),
            //    boost::gil::get_color(p, boost::gil::alpha_t())
            //});
            storage->push_back(PixelFormat{p[0], p[1], p[2], p[3]});
        }
    };
}

imageData loadImage_PNG_rgba8(const std::string& imagePath){
    boost::gil::rgba8_image_t image;
    boost::gil::png_read_image(imagePath, image);

    std::vector<Pixel::rgba8> pixelData;
    pixelData.reserve(image.width() * image.height() * boost::gil::num_channels<boost::gil::rgba8_image_t>());
    boost::gil::for_each_pixel(boost::gil::const_view(image), Pixel::Extractor<Pixel::rgba8>(&pixelData));

    return imageData{image.width(), image.height(), Pixel::Format::rgba8, pixelData};
}

imageData loadImage_PNG(const std::string& imagePath){
    return loadImage_PNG_rgba8(imagePath);
}

imageData loadImage(const std::string& imagePath){
    return loadImage_PNG(imagePath);
}