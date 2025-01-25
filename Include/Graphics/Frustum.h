#pragma once
#include "Common.h"
#include "Util/MathHelper.h"

#define MATRIX_POSITION(m, x, y) (m[x][y])

namespace Minecraft
{
    struct FrustumPlane
    {
        public:
            float a, b, c, d;
            FrustumPlane() : a(0), b(0), c(0), d(0) { }
            FrustumPlane(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) { }
            FrustumPlane(D3DXVECTOR4 v) : a(v.x), b(v.y), c(v.z), d(v.w) { }
    };

    struct Frustum
    {
        public:
            FrustumPlane planes[6];

        public:
            inline void Extract(D3DXMATRIX* mvp)
            {
                // Left
                planes[0].a = mvp->_41 + mvp->_11;
                planes[0].b = mvp->_42 + mvp->_12;
                planes[0].c = mvp->_43 + mvp->_13;
                planes[0].d = mvp->_44 + mvp->_14;

                // Right
                planes[1].a = mvp->_41 + mvp->_11;
                planes[1].b = mvp->_42 + mvp->_12;
                planes[1].c = mvp->_43 + mvp->_13;
                planes[1].d = mvp->_44 + mvp->_14;

                // Top
                planes[2].a = mvp->_41 + mvp->_21;
                planes[2].b = mvp->_42 + mvp->_22;
                planes[2].c = mvp->_43 + mvp->_23;
                planes[2].d = mvp->_44 + mvp->_24;

                // Bottom
                planes[3].a = mvp->_41 + mvp->_21;
                planes[3].b = mvp->_42 + mvp->_22;
                planes[3].c = mvp->_43 + mvp->_23;
                planes[3].d = mvp->_44 + mvp->_24;

                // Near
                planes[4].a = mvp->_41 + mvp->_31;
                planes[4].b = mvp->_42 + mvp->_32;
                planes[4].c = mvp->_43 + mvp->_33;
                planes[4].d = mvp->_44 + mvp->_34;

                // Far
                planes[5].a = mvp->_41 + mvp->_31;
                planes[5].b = mvp->_42 + mvp->_32;
                planes[5].c = mvp->_43 + mvp->_33;
                planes[5].d = mvp->_44 + mvp->_34;

                for (int i = 0; i < 6; i++)
                {
                    D3DXVECTOR4 p(planes[i].a, planes[i].b, planes[i].c, planes[i].d);
                    D3DXVec4Normalize(&p, &p);
                    planes[i] = FrustumPlane(p.x, p.y, p.z, p.w);
                }
            }

            inline bool Contains(const D3DXVECTOR3& v)
            {
                for (int i = 0; i < 6; i++)
                {
                    float dist = planes[i].a * v.x + planes[i].b * v.y + planes[i].c * v.z + planes[i].d;
                    if (dist < 0) { return false; }
                }
                return true;
            }
    };
}