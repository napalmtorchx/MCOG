#include "World/WorldGenerator.h"
#include "Minecraft.h"

namespace Minecraft
{
	void WorldGenerator::GenerateTerrain(World* world, int seed)
	{
        float* heightmap = new float[WORLD_WIDTH * WORLD_DEPTH];
        float  scale = 0.1f;

        fnl_state noisegen = fnlCreateState();
        noisegen.noise_type = FNL_NOISE_OPENSIMPLEX2;
        noisegen.seed = seed;

        int x, y, z;
        for (z = 0; z < WORLD_DEPTH; z++)
        {
            int r = rand() % WORLD_WIDTH;
            for (x = 0; x < WORLD_WIDTH; x++)
            {
                float noise = fnlGetNoise3D(&noisegen, (float)x * scale, 0, (float)z * scale);
                //float randomFactor = (rand() % 1000) / 1000.0f;  // Random value between 0 and 1
                //noise += randomFactor * 0.012f;
                heightmap[z * WORLD_DEPTH + x] = noise;
            }
        }
        RXDK_LOG_INFO("Finished generating heightmap noise\n");

        for (z = 0; z < WORLD_DEPTH_CHUNKS; z++)
        {
            for (y = 0; y < WORLD_HEIGHT_CHUNKS; y++)
            {
                for (x = 0; x < WORLD_WIDTH_CHUNKS; x++)
                {
                    int index = CHUNK_INDEX(x, y, z, WORLD_WIDTH_CHUNKS, WORLD_HEIGHT_CHUNKS);
                    Chunk* chunk = &world->chunks[index];
                    world->chunks[index] = Chunk();
                    *chunk = Chunk(x, y, z);
                    chunk->Load();
                    chunk->Fill(0);
                    chunk->index = index;
                    RXDK_LOG_INFO("Created chunk %d(%d, %d, %d)\n", index, x, y, z);

                    GenerateChunkTerrain(world, chunk, heightmap);
                    chunk->mesh = ChunkMesh(chunk);
                    chunk->mesh.Generate();
                    chunk->loaded = true;
                }
            }
        }

        delete[] heightmap;
	}

	void WorldGenerator::GenerateChunkTerrain(World* world, Chunk* chunk, float* heightmap)
	{
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            for (int x = 0; x < CHUNK_WIDTH; x++)
            {
                int zz = (chunk->pos.z * CHUNK_DEPTH) + z;
                int xx = (chunk->pos.x * CHUNK_WIDTH) + x;

                float noise = heightmap[zz * WORLD_DEPTH + xx];
                int   height = (WORLD_HEIGHT / 2) + (int)(noise * (CHUNK_HEIGHT * 2));
                int   dirt = height - 4;

                for (int y = 0; y < CHUNK_HEIGHT; y++)
                {
                    int i = BLOCK_INDEX(x, y, z);
                    int yy = y + (chunk->pos.y * CHUNK_HEIGHT);

                    if (yy == height) { chunk->blocks[i] = Block(1); }
                    else if (yy >= dirt && yy < height) { chunk->blocks[i] = Block(2); }
                    else if (yy < height) { chunk->blocks[i] = Block(3); }
                }
            }
        }
	}
}