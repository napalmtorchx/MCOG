#pragma once
#include "Common.h"

namespace Minecraft
{
    struct Ray
    {
        public:
            D3DXVECTOR3 origin;
            D3DXVECTOR3 direction;

        public:
            Ray() { origin = D3DXVECTOR3(0, 0, 0); direction = D3DXVECTOR3(0, 0, 0); }
            Ray(D3DXVECTOR3 origin, D3DXVECTOR3 dir) : origin(origin), direction(dir) { }
            bool Intersects(const D3DXVECTOR3& pos);
    };
}