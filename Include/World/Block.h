#pragma once
#include "Common.h"
#include "Graphics/Texture.h"
#include "Graphics/Voxel.h"

#define BLOCKID_AIR 0

namespace Minecraft
{
	typedef DWORD BlockID;

	struct Block
	{
		public:
			BlockID id;
			Block() : id(0) { }
			Block(BlockID id) : id(id) { }
	};

	class BlockMetadata
	{
		public:
			BlockID     id;
			std::string name;
			Texture*    textures[VOXEL_FACE_COUNT];

		public:
			BlockMetadata() : id(0), name("") { memset(textures, 0, sizeof(textures)); }
			BlockMetadata(BlockID id, std::string name, Texture* tex) : id(id), name(name) { CopyTexture(tex); }
			BlockMetadata(BlockID id, std::string name, Texture* tex_top, Texture* tex_btm, Texture* tex_sides) : id(id), name(name) { CopyTextures(tex_top, tex_btm, tex_sides); }
			BlockMetadata(BlockID id, std::string name, Texture** textures) : id(id), name(name) { CopyTextures(textures); }

		public:
			inline void CopyTextures(Texture** new_textures)
			{
				for (size_t i = 0; i < VOXEL_FACE_COUNT; i++) { textures[i] = new_textures[i]; }
			}

			inline void CopyTextures(Texture* tex_top, Texture* tex_btm, Texture* tex_sides)
			{
				for (int i = 0; i < VOXELFACE_TOP; i++) { textures[i] = tex_sides; }
				textures[VOXELFACE_TOP] = tex_top;
				textures[VOXELFACE_BOTTOM] = tex_btm;
			}

			inline void CopyTexture(Texture* tex) { for (int i = 0; i < VOXEL_FACE_COUNT; i++) { textures[i] = tex; } }
	};
}