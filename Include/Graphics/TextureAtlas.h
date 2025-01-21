#pragma once
#include "Graphics/Texture.h"

namespace Minecraft
{
    class TextureAtlas
    {
        public:
            std::vector<Texture> textures;

        private:
            D3DXVECTOR2 _sz;
            D3DXVECTOR2 _childsz;

        public:
            TextureAtlas();
            TextureAtlas(LPCSTR fname, int sw, int sh, D3DXCOLOR key);

        public:
            Texture* Get(int index);
            Texture* Get(int x, int y);

        public:
            D3DXVECTOR2 Size();
            D3DXVECTOR2 ChildSize();
    };
}
