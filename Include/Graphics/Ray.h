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
            bool Intersects(const D3DXVECTOR3& pos);
    };
}