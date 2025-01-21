#pragma once
#include "Common.h"
#include "Graphics/Texture.h"
#include "Graphics/Rectangle.h"

namespace Minecraft
{
    class SpriteFont
    {
        public:
            D3DXVECTOR2 sz;
            D3DXVECTOR2 spacing;
            D3DXVECTOR2 offset;
            Texture* texture;

        public:
            SpriteFont();
            SpriteFont(int cw, int ch, int sx, int sy, int ox, int oy, Texture* texture);
            SpriteFont(const D3DXVECTOR2& v_sz, const D3DXVECTOR2& v_spc, const D3DXVECTOR2& v_off, Texture* texture);

        public:
            Rectangle Glyph(char c);
    };
}