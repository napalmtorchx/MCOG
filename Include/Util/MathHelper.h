#pragma once
#include "Common.h"

namespace Minecraft
{
    class MathHelper
    {
        public:
            static inline float RadiansToDegrees(float radians)
            {
                return radians * (180.0f / D3DX_PI);
            }

            static inline float DegreesToRadians(float degrees)
            {
                return degrees * (D3DX_PI / 180.0f);
            }

            static inline float Magnitude(D3DXVECTOR3 v)
            {
                return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
            }
            
            static inline D3DXVECTOR3 Multiply(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
            {
                return D3DXVECTOR3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
            }

            static inline float Distance(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
            {
                float dx = v1.x - v2.x, dy = v1.y - v2.y, dz = v1.z - v2.z;
                return sqrtf(dx * dx + dy * dy + dz * dz);
            }

    };
}