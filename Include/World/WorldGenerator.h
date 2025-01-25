#pragma once
#include "Common.h"
#include "World/World.h"

namespace Minecraft
{
	class WorldGenerator
	{
		public:
			static void GenerateTerrain(World* world, int seed);
			static void GenerateChunkTerrain(World* world, Chunk* chunk, float* heightmap);
	};
}