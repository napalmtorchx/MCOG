#include "Graphics/SpriteFont.h"
#include "Minecraft.h"

namespace Minecraft
{
    SpriteFont::SpriteFont() : sz(0, 0), spacing(0, 0), offset(0, 0), texture(NULL)
    {

    }

    SpriteFont::SpriteFont(int cw, int ch, int sx, int sy, int ox, int oy, Texture* texture)
    {
        this->sz = D3DXVECTOR2(cw, ch);
        this->spacing = D3DXVECTOR2(sx, sy);
        this->offset = D3DXVECTOR2(ox, oy);
        this->texture = texture;
    }

    SpriteFont::SpriteFont(const D3DXVECTOR2& v_sz, const D3DXVECTOR2& v_spc, const D3DXVECTOR2& v_off, Texture* texture)
    {
        this->sz = v_sz;
        this->spacing = v_spc;
        this->offset = v_off;
        this->texture = texture;
    }

    Rectangle SpriteFont::Glyph(char c)
    {
        if (c < 32 || c > 126) { return Rectangle(); }
        return Rectangle((int)offset.x + (((int)c - 32) * (int)sz.x), (int)offset.y, (int)sz.x, (int)sz.y);
    }
}