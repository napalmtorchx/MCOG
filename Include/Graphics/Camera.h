#pragma once
#include "Common.h"

#define CLIPPING_NEAR 0
#define CLIPPING_FAR  1

namespace Minecraft
{
    class Camera
    {
        public:
            D3DXVECTOR3 pos, up;
            D3DXVECTOR2 lookdir;
            D3DXMATRIX  proj_matrix, view_matrix, mvp_matrix;
            float    fov;
            float    clipping[2];

        private:
            D3DXMATRIX _m_trans;

        public:
            Camera();
            Camera(float x, float y, float z, float lx, float ly, float fov, float zn, float zf);
            Camera(const D3DXVECTOR3& pos, const D3DXVECTOR2& lookdir, float fov, float zn, float zf);

        public:
            void Update(float dt);
            void Project();

    };
}