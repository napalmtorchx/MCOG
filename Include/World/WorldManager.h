#pragma once
#include "Common.h"
#include "World/World.h"

namespace Minecraft
{
	class WorldManager
	{
		public:
			static World* LoadWorld(LPCSTR path);
			static Chunk* LoadChunk(LPCSTR fname);
			static void SaveWorld(LPCSTR path, World* world);
			static void SaveChunk(LPCSTR fname, Chunk* chunk);
	};
}
