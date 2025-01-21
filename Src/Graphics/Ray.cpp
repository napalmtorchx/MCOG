#include "Graphics/Ray.h"
#include "Minecraft.h"

namespace Minecraft
{
    bool Ray::Intersects(const D3DXVECTOR3& pos)
    {
        float tmin = -FLT_MAX;
        float tmax = FLT_MAX;
        float dir[3] = { direction.x, direction.y, direction.z };
        float org[3] = { origin.x,    origin.y,    origin.z };
        float minbnds[3] = { pos.x,       pos.y,       pos.z };
        float maxbnds[3] = { pos.x + 1,   pos.y + 1,   pos.z + 1 };

        for (int i = 0; i < 3; i++)
        {
            if (fabs(dir[i]) < FLT_EPSILON)
            {
                if (org[i] < minbnds[i] || org[i] > maxbnds[i]) { return false; }
            }
            else
            {
                float t1 = (minbnds[i] - org[i]) / dir[i];
                float t2 = (maxbnds[i] - org[i]) / dir[i];
                tmin = max(tmin, min(t1, t2));
                tmax = min(tmax, max(t1, t2));
                if (tmin > tmax) { return false; }
            }
        }
        return true;
    }
}