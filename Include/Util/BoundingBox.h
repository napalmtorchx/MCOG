#pragma once
#include "Common.h"

namespace Minecraft
{
    struct BoundingBox
    {
        public:
            D3DXVECTOR3 minimum;
            D3DXVECTOR3 maximum;

            BoundingBox() : minimum(0, 0, 0), maximum(0, 0, 0) {}
            BoundingBox(const D3DXVECTOR3& min, const D3DXVECTOR3& max) : minimum(min), maximum(max) {}
            BoundingBox(float x_min, float y_min, float z_min, float x_max, float y_max, float z_max) : minimum(x_min, y_min, z_min), maximum(x_max, y_max, z_max) {}
        };
}