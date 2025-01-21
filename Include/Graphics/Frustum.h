#pragma once
#include "Common.h"

namespace Minecraft
{
    struct Frustum
    {
        public:
            float planes[6][4];

        public:
            void Extract(D3DXMATRIX* mvp);
            bool Contains(const D3DXVECTOR3& v);
    };
}