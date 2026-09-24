#include "TextureLoader.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <png.h>

namespace fs = std::filesystem;

GLuint TextureLoader::loadPNG(const std::string& path)
{
    FILE* file = fopen(path.c_str(), "rb");

    if (!file)
        return 0;

    png_structp png = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        nullptr,
        nullptr
    );

    if (!png) {
        fclose(file);
        return 0;
    }

    png_infop info = png_create_info_struct(png);

    if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(file);
        return 0;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(file);
        return 0;
    }

    png_init_io(png, file);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    int colorType = png_get_color_type(png, info);
    int bitDepth = png_get_bit_depth(png, info);

    if (bitDepth == 16)
        png_set_strip_16(png);

    if (colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (colorType == PNG_COLOR_TYPE_RGB ||
        colorType == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (colorType == PNG_COLOR_TYPE_GRAY ||
        colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    std::vector<unsigned char> pixels(
        width * height * 4
    );

    std::vector<png_bytep> rows(height);

    for (int y = 0; y < height; y++)
        rows[y] = pixels.data() + y * width * 4;

    png_read_image(png, rows.data());

    fclose(file);
    png_destroy_read_struct(&png, &info, nullptr);

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

bool TextureLoader::loadFolder(const std::string& folder)
{
    if (!fs::exists(folder))
        return false;

    for (const auto& entry : fs::directory_iterator(folder))
    {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() != ".png")
            continue;

        std::string name =
            entry.path().stem().string();

        GLuint texture =
            loadPNG(entry.path().string());

        if (texture == 0)
            continue;

        textures[name] = texture;
    }

    return true;
}

GLuint TextureLoader::get(const std::string& name) const
{
    auto it = textures.find(name);

    if (it == textures.end())
        return 0;

    return it->second;
}
