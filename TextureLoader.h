#pragma once

#include <string>
#include <unordered_map>
#include <GL/gl.h>

class TextureLoader {
public:
    bool loadFolder(const std::string& folder);
    GLuint get(const std::string& name) const;

private:
    std::unordered_map<std::string, GLuint> textures;

    GLuint loadPNG(const std::string& path);
};
