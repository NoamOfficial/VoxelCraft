#pragma once

#include "Chunk.h"
#include "TextureLoader.h"
#include <cstdint>

class ChunkGenerator {
public:
    explicit ChunkGenerator(uint32_t seed);

    void generate(Chunk& chunk);

private:
    uint32_t seed;

    float noise2D(int x, int z) const;
    float noise3D(int x, int y, int z) const;

    int getTerrainHeight(int worldX, int worldZ) const;

    BlockID getBlockForDepth(
        int worldX,
        int worldY,
        int worldZ,
        int surfaceY
    ) const;
};
