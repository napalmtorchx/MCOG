#include "Graphics/Camera.h"
#include "Minecraft.h"

namespace Minecraft
{
    Camera::Camera()
    {
        this->up = D3DXVECTOR3(0, 1, 0);
        this->pos = D3DXVECTOR3(0, 0, 0);
        this->lookdir = D3DXVECTOR2(0, 0);
        this->fov = 0.0f;
        this->clipping[CLIPPING_NEAR] = 0.0f;
        this->clipping[CLIPPING_FAR] = 0.0f;
    }

    Camera::Camera(float x, float y, float z, float lx, float ly, float fov, float zn, float zf)
    {
        this->up = D3DXVECTOR3(0, 1, 0);
        this->pos = D3DXVECTOR3(x, y, z);
        this->lookdir = D3DXVECTOR2(lx, ly);
        this->fov = fov;
        this->clipping[CLIPPING_NEAR] = zn;
        this->clipping[CLIPPING_FAR] = zf;
    }

    Camera::Camera(const D3DXVECTOR3& pos, const D3DXVECTOR2& lookdir, float fov, float zn, float zf)
    {
        this->up = D3DXVECTOR3(0, 1, 0);
        this->pos = pos;
        this->lookdir = lookdir;
        this->fov = fov;
        this->clipping[CLIPPING_NEAR] = zn;
        this->clipping[CLIPPING_FAR] = zf;
    }

    void Camera::Update(float dt)
    {
        // World matrix
        D3DXMATRIX m_world;
        D3DXMatrixIdentity(&m_world);

        // View matrix
        D3DXVECTOR3 forward = D3DXVECTOR3(pos.x + cosf(lookdir.x), pos.y + lookdir.y, pos.z + sinf(lookdir.x));
        D3DXMatrixLookAtLH(&view_matrix, &pos, &forward, &up);

        // Projection matrix
        float f = MathHelper::DegreesToRadians(fov);
        float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
        D3DXMatrixPerspectiveFovLH(&proj_matrix, f, aspect, clipping[CLIPPING_NEAR], clipping[CLIPPING_FAR]);

        // Send multiplied matrices to renderer
        _m_trans = view_matrix * proj_matrix;
        _m_trans *= m_world;
        D3DXMatrixTranspose(&_m_trans, &_m_trans);
        mvp_matrix = _m_trans;
    }

    void Camera::Project()
    {
        float zeros[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        GraphicsManager::device->SetVertexShaderConstant(0, zeros, 4);

        D3DXMATRIX world_matrix, worldview_matrix;
        D3DXMatrixIdentity(&world_matrix);

        worldview_matrix = world_matrix * view_matrix;

        D3DXMatrixTranspose(&world_matrix, &world_matrix);
        D3DXMatrixTranspose(&worldview_matrix, &worldview_matrix);

        GraphicsManager::device->SetVertexShaderConstant(10, &world_matrix, 4);
        GraphicsManager::device->SetVertexShaderConstant(20, &worldview_matrix, 4);
        GraphicsManager::device->SetVertexShaderConstant(30, &mvp_matrix, 4);
    }
}