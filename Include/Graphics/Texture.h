#pragma once
#include "Common.h"

namespace Minecraft
{
    class Texture
    {
    private:
        LPDIRECT3DTEXTURE8 _d3d;
        D3DXVECTOR2        _sz;

    public:
        Texture();
        Texture(LPCSTR fname, D3DXCOLOR key = 0x00FFFFFF);
        Texture(LPDIRECT3DTEXTURE8 texture);

    public:
        LPDIRECT3DTEXTURE8 D3D();
        D3DXVECTOR2 Size();
    };
}