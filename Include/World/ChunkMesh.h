#pragma once
#include "Common.h"
#include "Graphics/Texture.h"
#include "Graphics/Vertex.h"
#include "Graphics/Voxel.h"
#include "World/Block.h"

namespace Minecraft
{
    class Chunk;

    struct ChunkMeshFaceKey
    {
        public:
            BYTE face;
            int  index;

        public:
            ChunkMeshFaceKey();
            ChunkMeshFaceKey(int index, BYTE face);
            bool operator<(const ChunkMeshFaceKey& other) const;
    };

    class ChunkMeshFace
    {
        public:
            BYTE        face;
            BYTE        id;
            Texture*    texture;
            D3DXVECTOR3 sz;
            Vertex      vertices[FACE_VERTEX_COUNT];

        public:
            ChunkMeshFace();
            ChunkMeshFace(BYTE face, BYTE type, Texture* texture);
    };

    class ChunkMesh
    {
        public:
            Chunk* chunk;
            std::map<ChunkMeshFaceKey, ChunkMeshFace> facemap;
            std::vector<ChunkMeshFace>                faces;
            bool generated;

        private:
            ChunkMeshFace  _tempfaces[VOXEL_FACE_COUNT];
            ChunkMeshFace* _sidefaces[VOXEL_FACE_COUNT * 2];

        public:
            ChunkMesh();
            ChunkMesh(Chunk* chunk);
            void Generate();
            void GenerateBlockFaces(BlockMetadata* metadata, Block* blk, int x, int y, int z);
            void GreedyMesh();
            void GreedyMeshX(int x, int y, int z);
            void GreedyMeshY(int x, int y, int z);
            void GreedyMeshZ(int x, int y, int z);

        public:
            int GetNeighbors(Block** blks, int x, int y, int z);
            ChunkMeshFace* GetFace(int x, int y, int z, BYTE face);
        };
}