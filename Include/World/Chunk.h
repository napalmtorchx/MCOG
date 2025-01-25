#pragma once
#include "Common.h"
#include "Engine/Component.h"
#include "World/Block.h"
#include "World/ChunkMesh.h"

#define CHUNK_WIDTH  16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH  16
#define CHUNK_SIZE   (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH)

#define MAX_CHUNKS_RENDERED_X 6
#define MAX_CHUNKS_RENDERED_Y 6
#define MAX_CHUNKS_RENDERED_Z 6
#define MAX_CHUNKS_RENDERED   (MAX_CHUNKS_RENDERED_X * MAX_CHUNKS_RENDERED_Y * MAX_CHUNKS_RENDERED_Z)

#define BLOCK_INDEX(x, y, z) (x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z))
#define BLOCK_X(i, z, y)     (i - z * CHUNK_WIDTH * CHUNK_HEIGHT - y * CHUNK_WIDTH)
#define BLOCK_Y(i, z)        ((i - z * CHUNK_WIDTH * CHUNK_HEIGHT) / CHUNK_WIDTH)
#define BLOCK_Z(i)           (i / (CHUNK_WIDTH * CHUNK_HEIGHT))

namespace Minecraft
{
	class Chunk : public Component
	{
		public:
			ChunkMesh   mesh;
			Block*      blocks;
			D3DXVECTOR3 pos;
			int			index;
			bool        loaded;
			bool		contains_player;
			bool		rayhit;
			float		dist_to_player;
			
		private:
			size_t _rend_faces, _total_faces;

		public:
			Chunk();
			Chunk(int x, int y, int z);
			void Load() override;
			void Unload() override;
			void Update(float dt) override;
			void UpdateFixed() override;
			void Draw(float dt) override;

		public:
			void Fill(BlockID id);
			void Set(int x, int y, int z, BlockID id);
			Block* Get(int x, int y, int z);

		public:
			size_t GetRenderedFaceCount();
			size_t GetTotalFaceCount();
	};
}