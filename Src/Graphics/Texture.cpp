#include "Graphics/Texture.h"
#include "Minecraft.h"

namespace Minecraft
{
    Texture::Texture() : _d3d(NULL), _sz(0, 0) {}

    Texture::Texture(LPCSTR fname, D3DXCOLOR key) : _d3d(NULL), _sz(0, 0)
    {
        HRESULT res = D3DXCreateTextureFromFileExA(GraphicsManager::device, fname, 0, 0, 1, 0, D3DFMT_UNKNOWN, 0, D3DX_FILTER_POINT, D3DX_FILTER_POINT, key, NULL, NULL, &_d3d);
        if (FAILED(res)) { RXDK_LOG_ERROR("Failed to locate texture at '%s'", fname); return; }

        D3DSURFACE_DESC desc;
        _d3d->GetLevelDesc(0, &desc);
        _sz = D3DXVECTOR2((int)desc.Width, (int)desc.Height);
    }

    Texture::Texture(LPDIRECT3DTEXTURE8 texture)
    {
        _d3d = texture;
        if (_d3d == NULL) { _sz = D3DXVECTOR2(0, 0); return; }

        D3DSURFACE_DESC desc;
        _d3d->GetLevelDesc(0, &desc);
        _sz = D3DXVECTOR2((int)desc.Width, (int)desc.Height);
    }

    LPDIRECT3DTEXTURE8 Texture::D3D() { return _d3d; }

    D3DXVECTOR2 Texture::Size() { return _sz; }
}