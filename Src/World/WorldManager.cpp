#include "World/WorldManager.h"
#include "Minecraft.h"

namespace Minecraft
{
	World* WorldManager::LoadWorld(LPCSTR path)
	{
		return NULL;
	}

	Chunk* WorldManager::LoadChunk(LPCSTR fname)
	{
		return NULL;
	}

	void WorldManager::SaveWorld(LPCSTR path, World* world)
	{

	}

	void WorldManager::SaveChunk(LPCSTR fname, Chunk* chunk)
	{
		std::vector<BYTE> data;

		FileSystem::WriteBinary(fname, &data[0], data.size());
	}
}