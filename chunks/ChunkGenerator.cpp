#include "ChunkGenerator.h"
#include <cmath>
#include <algorithm>

ChunkGenerator::ChunkGenerator(uint32_t seed)
    : seed(seed) {
}

float ChunkGenerator::noise2D(int x, int z) const {
    uint32_t n =
        static_cast<uint32_t>(x * 374761393) ^
        static_cast<uint32_t>(z * 668265263) ^
        seed;

    n = (n ^ (n >> 13)) * 1274126177u;
    n ^= n >> 16;

    return static_cast<float>(n) /
           static_cast<float>(UINT32_MAX);
}

float ChunkGenerator::noise3D(int x, int y, int z) const {
    uint32_t n =
        static_cast<uint32_t>(x * 374761393) ^
        static_cast<uint32_t>(y * 1103515245) ^
        static_cast<uint32_t>(z * 668265263) ^
        seed;

    n = (n ^ (n >> 13)) * 1274126177u;
    n ^= n >> 16;

    return static_cast<float>(n) /
           static_cast<float>(UINT32_MAX);
}

int ChunkGenerator::getTerrainHeight(int worldX, int worldZ) const {
    float n = noise2D(worldX / 8, worldZ / 8);

    // 0–1024 terrain range
    return 48 + static_cast<int>(n * 32.0f);
}

BlockID ChunkGenerator::getBlockForDepth(
    int worldX,
    int worldY,
    int worldZ,
    int surfaceY
) const {
    int depth = surfaceY - worldY;

    // Surface
    if (depth == 0)
        return GRASS;

    // Dirt layer
    if (depth <= 2)
        return DIRT;

    // Stone starts 3 blocks below surface
    if (depth < 13)
        return STONE;

    // 3D cave noise
    float caveNoise = noise3D(
        worldX / 4,
        worldY / 4,
        worldZ / 4
    );

    // Caves get smaller with depth
    float caveThreshold;

    if (depth < 18)
        caveThreshold = 0.82f;
    else
        caveThreshold = 0.90f;

    if (caveNoise > caveThreshold)
        return AIR;

    // Ore generation
    float oreNoise = noise3D(
        worldX,
        worldY,
        worldZ
    );

    if (depth >= 13) {
        if (oreNoise > 0.995f)
            return DIAMOND_ORE;

        if (oreNoise > 0.985f)
            return EMERALD_ORE;

        if (oreNoise > 0.970f)
            return GOLD_ORE;

        if (oreNoise > 0.940f)
            return REDSTONE_ORE;

        if (oreNoise > 0.900f)
            return IRON_ORE;
    }

    return STONE;
}

void ChunkGenerator::generate(Chunk& chunk) {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {

            int worldX = chunk.x * CHUNK_SIZE + x;
            int worldZ = chunk.z * CHUNK_SIZE + z;

            int surfaceY =
                getTerrainHeight(worldX, worldZ);

            for (int y = 0; y < CHUNK_HEIGHT; ++y) {

                if (y > surfaceY) {
                    chunk.blocks[x][y][z] = AIR;
                    continue;
                }

                chunk.blocks[x][y][z] =
                    getBlockForDepth(
                        worldX,
                        y,
                        worldZ,
                        surfaceY
                    );
            }
        }
    }

    // Newly generated chunks don't need saving yet.
    chunk.modified = false;
}
