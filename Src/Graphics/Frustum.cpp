#include "Graphics/Frustum.h"
#include "Minecraft.h"

namespace Minecraft
{
    void Frustum::Extract(D3DXMATRIX* mvp)
    {
        D3DXMATRIX& mat = *mvp;

        planes[0][0] = MATRIX_POSITION(mat.m, 0, 3) + MATRIX_POSITION(mat.m, 0, 0);
        planes[0][1] = MATRIX_POSITION(mat.m, 1, 3) + MATRIX_POSITION(mat.m, 1, 0);
        planes[0][2] = MATRIX_POSITION(mat.m, 2, 3) + MATRIX_POSITION(mat.m, 2, 0);
        planes[0][3] = MATRIX_POSITION(mat.m, 3, 3) + MATRIX_POSITION(mat.m, 3, 0);

        planes[1][0] = MATRIX_POSITION(mat.m, 0, 3) + MATRIX_POSITION(mat.m, 0, 0);
        planes[1][1] = MATRIX_POSITION(mat.m, 1, 3) + MATRIX_POSITION(mat.m, 1, 0);
        planes[1][2] = MATRIX_POSITION(mat.m, 2, 3) + MATRIX_POSITION(mat.m, 2, 0);
        planes[1][3] = MATRIX_POSITION(mat.m, 3, 3) + MATRIX_POSITION(mat.m, 3, 0);

        planes[2][0] = MATRIX_POSITION(mat.m, 0, 3) + MATRIX_POSITION(mat.m, 0, 1);
        planes[2][1] = MATRIX_POSITION(mat.m, 1, 3) + MATRIX_POSITION(mat.m, 1, 1);
        planes[2][2] = MATRIX_POSITION(mat.m, 2, 3) + MATRIX_POSITION(mat.m, 2, 1);
        planes[2][3] = MATRIX_POSITION(mat.m, 3, 3) + MATRIX_POSITION(mat.m, 3, 1);

        planes[3][0] = MATRIX_POSITION(mat.m, 0, 3) + MATRIX_POSITION(mat.m, 0, 1);
        planes[3][1] = MATRIX_POSITION(mat.m, 1, 3) + MATRIX_POSITION(mat.m, 1, 1);
        planes[3][2] = MATRIX_POSITION(mat.m, 2, 3) + MATRIX_POSITION(mat.m, 2, 1);
        planes[3][3] = MATRIX_POSITION(mat.m, 3, 3) + MATRIX_POSITION(mat.m, 3, 1);

        planes[4][0] = MATRIX_POSITION(mat.m, 0, 3) + MATRIX_POSITION(mat.m, 0, 2);
        planes[4][1] = MATRIX_POSITION(mat.m, 1, 3) + MATRIX_POSITION(mat.m, 1, 2);
        planes[4][2] = MATRIX_POSITION(mat.m, 2, 3) + MATRIX_POSITION(mat.m, 2, 2);
        planes[4][3] = MATRIX_POSITION(mat.m, 3, 3) + MATRIX_POSITION(mat.m, 3, 2);

        planes[5][0] = MATRIX_POSITION(mat.m, 0, 3) + MATRIX_POSITION(mat.m, 0, 2);
        planes[5][1] = MATRIX_POSITION(mat.m, 1, 3) + MATRIX_POSITION(mat.m, 1, 2);
        planes[5][2] = MATRIX_POSITION(mat.m, 2, 3) + MATRIX_POSITION(mat.m, 2, 2);
        planes[5][3] = MATRIX_POSITION(mat.m, 3, 3) + MATRIX_POSITION(mat.m, 3, 2);

        for (int i = 0; i < 6; ++i)
        {
            float length = MathHelper::Magnitude(D3DXVECTOR3(planes[i][0], planes[i][1], planes[i][2]));
            planes[i][0] /= length;
            planes[i][1] /= length;
            planes[i][2] /= length;
            planes[i][3] /= length;
        }
    }

    bool Frustum::Contains(const D3DXVECTOR3& v)
    {
        for (int i = 0; i < 6; i++)
        {
            float dist = planes[i][0] * v.x + planes[i][1] * v.y + planes[i][2] * v.z + planes[i][3];
            if (dist < 0) { return false; }
        }
        return true;
    }
}