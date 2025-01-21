#pragma once
#include "Common.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)

namespace Minecraft
{
    struct Vertex
    {
        D3DXVECTOR3 pos;
        DWORD       color;
        D3DXVECTOR2 uv;
    };

    static const DWORD VERTEX_SHADER_DECL[] =
    {
        D3DVSD_STREAM(0),
        D3DVSD_REG(0, D3DVSDT_FLOAT3),   // Position
        D3DVSD_REG(1, D3DVSDT_D3DCOLOR), // Diffuse color
        D3DVSD_REG(6, D3DVSDT_FLOAT2),   // UV
        D3DVSD_END()
    };
}