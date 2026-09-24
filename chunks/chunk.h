#pragma once

#include <cstdint>

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 128;

enum BlockID : uint8_t {
    AIR = 0,
    GRASS,
    DIRT,
    STONE,
    COBBLESTONE
};

struct Chunk {
    int x;
    int z;

    BlockID blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];

    bool modified = false;

    Chunk(int chunkX, int chunkZ)
        : x(chunkX), z(chunkZ)
    {
        for (int bx = 0; bx < CHUNK_SIZE; bx++)
            for (int y = 0; y < CHUNK_HEIGHT; y++)
                for (int bz = 0; bz < CHUNK_SIZE; bz++)
                    blocks[bx][y][bz] = AIR;
    }

    BlockID getBlock(int x, int y, int z) const {
        if (x < 0 || x >= CHUNK_SIZE ||
            y < 0 || y >= CHUNK_HEIGHT ||
            z < 0 || z >= CHUNK_SIZE)
            return AIR;

        return blocks[x][y][z];
    }

    void setBlock(int x, int y, int z, BlockID block) {
        if (x < 0 || x >= CHUNK_SIZE ||
            y < 0 || y >= CHUNK_HEIGHT ||
            z < 0 || z >= CHUNK_SIZE)
            return;

        if (blocks[x][y][z] == block)
            return;

        blocks[x][y][z] = block;
        modified = true;
    }
};
