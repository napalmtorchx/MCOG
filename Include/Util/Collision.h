#pragma once
#include "Common.h"
#include "Util/BoundingBox.h"

namespace Minecraft
{
    class Collision
    {
        public:
            static inline bool CheckLeft(D3DXVECTOR3 pos1, D3DXVECTOR3 sz1, D3DXVECTOR3 pos2, D3DXVECTOR3 sz2, float spd)
            {
                BoundingBox a(pos1.x, pos1.y, pos1.z, pos1.x + sz1.x, pos1.y + sz1.y, pos1.z + sz1.z);
                BoundingBox b(pos2.x, pos2.y, pos2.z, pos2.x + sz2.x, pos2.y + sz2.y, pos2.z + sz2.z);

                return  a.maximum.x + spd > b.minimum.x &&
                    a.minimum.x       < b.minimum.x &&
                    a.maximum.y       > b.minimum.y &&
                    a.minimum.y       < b.maximum.y &&
                    a.maximum.z       > b.minimum.z &&
                    a.minimum.z < b.maximum.z;
            }

            static inline bool CheckRight(D3DXVECTOR3 pos1, D3DXVECTOR3 sz1, D3DXVECTOR3 pos2, D3DXVECTOR3 sz2, float spd)
            {
                BoundingBox a(pos1.x, pos1.y, pos1.z, pos1.x + sz1.x, pos1.y + sz1.y, pos1.z + sz1.z);
                BoundingBox b(pos2.x, pos2.y, pos2.z, pos2.x + sz2.x, pos2.y + sz2.y, pos2.z + sz2.z);

                return  a.minimum.x + spd < b.maximum.x &&
                    a.maximum.x       > b.maximum.x &&
                    a.maximum.y > b.minimum.y &&
                    a.minimum.y       < b.maximum.y &&
                    a.maximum.z       > b.minimum.z &&
                    a.minimum.z < b.maximum.z;
            }

            static inline bool CheckTop(D3DXVECTOR3 pos1, D3DXVECTOR3 sz1, D3DXVECTOR3 pos2, D3DXVECTOR3 sz2, float spd)
            {
                BoundingBox a(pos1.x, pos1.y, pos1.z, pos1.x + sz1.x, pos1.y + sz1.y, pos1.z + sz1.z);
                BoundingBox b(pos2.x, pos2.y, pos2.z, pos2.x + sz2.x, pos2.y + sz2.y, pos2.z + sz2.z);

                return  a.maximum.y + spd > b.minimum.y &&
                    a.minimum.y       < b.minimum.y &&
                    a.maximum.x       > b.minimum.x &&
                    a.minimum.x       < b.maximum.x &&
                    a.maximum.z       > b.minimum.z &&
                    a.minimum.z < b.maximum.z;
            }

            static inline bool CheckBottom(D3DXVECTOR3 pos1, D3DXVECTOR3 sz1, D3DXVECTOR3 pos2, D3DXVECTOR3 sz2, float spd)
            {
                BoundingBox a(pos1.x, pos1.y, pos1.z, pos1.x + sz1.x, pos1.y + sz1.y, pos1.z + sz1.z);
                BoundingBox b(pos2.x, pos2.y, pos2.z, pos2.x + sz2.x, pos2.y + sz2.y, pos2.z + sz2.z);

                return  a.minimum.y + spd < b.maximum.y &&
                    a.maximum.y       > b.maximum.y &&
                    a.maximum.x > b.minimum.x &&
                    a.minimum.x       < b.maximum.x &&
                    a.maximum.z       > b.minimum.z &&
                    a.minimum.z < b.maximum.z;
            }

            static inline bool CheckFront(D3DXVECTOR3 pos1, D3DXVECTOR3 sz1, D3DXVECTOR3 pos2, D3DXVECTOR3 sz2, float spd)
            {
                BoundingBox a(pos1.x, pos1.y, pos1.z, pos1.x + sz1.x, pos1.y + sz1.y, pos1.z + sz1.z);
                BoundingBox b(pos2.x, pos2.y, pos2.z, pos2.x + sz2.x, pos2.y + sz2.y, pos2.z + sz2.z);

                return  a.maximum.z + spd > b.minimum.z &&
                    a.minimum.z       < b.minimum.z &&
                    a.maximum.x       > b.minimum.x &&
                    a.minimum.x       < b.maximum.x &&
                    a.maximum.y       > b.minimum.y &&
                    a.minimum.y < b.maximum.y;
            }

            static inline bool CheckBack(D3DXVECTOR3 pos1, D3DXVECTOR3 sz1, D3DXVECTOR3 pos2, D3DXVECTOR3 sz2, float spd)
            {
                BoundingBox a(pos1.x, pos1.y, pos1.z, pos1.x + sz1.x, pos1.y + sz1.y, pos1.z + sz1.z);
                BoundingBox b(pos2.x, pos2.y, pos2.z, pos2.x + sz2.x, pos2.y + sz2.y, pos2.z + sz2.z);

                return  a.minimum.z + spd < b.maximum.z &&
                    a.maximum.z       > b.maximum.z &&
                    a.maximum.x > b.minimum.x &&
                    a.minimum.x       < b.maximum.x &&
                    a.maximum.y       > b.minimum.y &&
                    a.minimum.y < b.maximum.y;
            }
    };
}