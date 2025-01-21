#include "Graphics/TextureAtlas.h"
#include "Minecraft.h"

namespace Minecraft
{
    TextureAtlas::TextureAtlas()
    {
        _sz = D3DXVECTOR2(0, 0);
        _childsz = D3DXVECTOR2(0, 0);
    }

    TextureAtlas::TextureAtlas(LPCSTR fname, int sw, int sh, D3DXCOLOR key)
    {
        Texture maintex = Texture(fname, key);

        D3DSURFACE_DESC desc;
        maintex.D3D()->GetLevelDesc(0, &desc);
        _sz = D3DXVECTOR2((int)desc.Width, (int)desc.Height);
        _childsz = D3DXVECTOR2(sw, sh);

        D3DLOCKED_RECT lkr_main;
        maintex.D3D()->LockRect(0, &lkr_main, NULL, 0);

        DWORD* main_unsw = new DWORD[desc.Width * desc.Height];
        XGUnswizzleRect(lkr_main.pBits, desc.Width, desc.Height, NULL, main_unsw, lkr_main.Pitch, NULL, sizeof(DWORD));

        D3DXVECTOR2 count = D3DXVECTOR2(_sz.x / _childsz.x, _sz.y / _childsz.y);
        for (int i = 0; i < count.x * count.y; i++)
        {
            LPDIRECT3DTEXTURE8 subtex;
            HRESULT res = D3DXCreateTexture(GraphicsManager::device, sw, sh, 1, 0, desc.Format, D3DPOOL_MANAGED, &subtex);

            D3DLOCKED_RECT lkr_sub;
            subtex->LockRect(0, &lkr_sub, NULL, 0);

            DWORD* sub_unsw = new DWORD[sw * sh];
            XGUnswizzleRect(lkr_sub.pBits, sw, sh, NULL, sub_unsw, lkr_sub.Pitch, NULL, sizeof(DWORD));

            for (int y = 0; y < sh; y++)
            {
                for (int x = 0; x < sw; x++)
                {
                    int main_x = x + ((i % (int)count.x) * sw);
                    int main_y = y + ((i / (int)count.x) * sh);
                    int src_i = main_y * desc.Width + main_x;

                    sub_unsw[(y * sw + x)] = main_unsw[src_i];
                }
            }

            XGSwizzleRect(sub_unsw, lkr_sub.Pitch, NULL, lkr_sub.pBits, sw, sh, NULL, sizeof(DWORD));
            subtex->UnlockRect(0);
            textures.push_back(Texture(subtex));;
            delete[] sub_unsw;
        }

        XGSwizzleRect(main_unsw, lkr_main.Pitch, NULL, lkr_main.pBits, desc.Width, desc.Height, NULL, sizeof(DWORD));

        maintex.D3D()->UnlockRect(0);
        delete[] main_unsw;
        maintex.D3D()->Release();

        RXDK_LOG_INFO("Loaded texture atlas - Size:%dx%d - Textures:%u\n", (int)_sz.x, (int)_sz.y, textures.size());
    }

    Texture* TextureAtlas::Get(int index)
    {
        if (_sz.x == 0 || _sz.y == 0 || _childsz.x == 0 || _childsz.y == 0) { return NULL; }

        if (index < 0 || index >= _sz.x * _sz.y) { return NULL; }
        return &textures[index];
    }

    Texture* TextureAtlas::Get(int x, int y)
    {
        if (_sz.x == 0 || _sz.y == 0 || _childsz.x == 0 || _childsz.y == 0) { return NULL; }

        D3DXVECTOR2 count(_sz.x / _childsz.x, _sz.y / _childsz.y);
        if (x < 0 || x >= count.x || y < 0 || y >= count.y) { return NULL; }
        return &textures[y * count.x + x];
    }

    D3DXVECTOR2 TextureAtlas::Size() { return _sz; }

    D3DXVECTOR2 TextureAtlas::ChildSize() { return _childsz; }
}