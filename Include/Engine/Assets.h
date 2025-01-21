#pragma once
#include "Common.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureAtlas.h"
#include "Graphics/SpriteFont.h"
#include "World/Block.h"

namespace Minecraft
{
    class Assets
    {
        public:
            static std::map<std::string, Texture> textures;
            static TextureAtlas                   atlas;
            static SpriteFont                     font;
            static std::map<int, BlockMetadata>   blocks;

        public:
            static void Init();

        public:
            static void LoadBlocks();
            static void LoadTexture(const std::string& name, LPCSTR fname, D3DXCOLOR key = 0x00FFFFFF);
    };
}