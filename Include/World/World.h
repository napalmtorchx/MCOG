#pragma once
#include "Common.h"
#include "World/Block.h"
#include "World/Chunk.h"

#define WORLD_WIDTH  32
#define WORLD_HEIGHT 64
#define WORLD_DEPTH  32
#define WORLD_SIZE   (WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH)

#define WORLD_WIDTH_CHUNKS  (WORLD_WIDTH  / CHUNK_WIDTH)
#define WORLD_HEIGHT_CHUNKS (WORLD_HEIGHT / CHUNK_HEIGHT)
#define WORLD_DEPTH_CHUNKS  (WORLD_DEPTH  / CHUNK_DEPTH)
#define WORLD_SIZE_CHUNKS   (WORLD_WIDTH_CHUNKS * WORLD_HEIGHT_CHUNKS * WORLD_DEPTH_CHUNKS)

#define CHUNK_INDEX(x, y, z, w, h) (x + w * (y + h * z))
#define CHUNK_X(i, z, y, w, h)     (i - z * w * h - y * w)
#define CHUNK_Y(i, z, w, h)        ((i - z * w * h) / w)
#define CHUNK_Z(i, w, h)           (i / (w * h))

namespace Minecraft
{
    class World : public Component
    {
        public:
            Chunk* chunks;
            Chunk* this_chunk;

        private:
            size_t _rend_faces, _total_faces;

        public:
            World();
            World(LPCSTR name);
            void Load() override;
            void Unload() override;
            void Update(float dt) override;
            void UpdateFixed() override;
            void Draw(float dt) override;

        public:
            void SetBlock(int x, int y, int z, BYTE type);
            Block* GetBlock(int x, int y, int z, Chunk** chunk);
            size_t GetRenderedFaceCount();
            size_t GetTotalFaceCount();
        };
}