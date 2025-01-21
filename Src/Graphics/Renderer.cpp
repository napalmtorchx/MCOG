#include "Graphics/Renderer.h"
#include "Minecraft.h"

namespace Minecraft
{
    Texture* Renderer::skybox[VOXEL_FACE_COUNT];
    Vertex   Renderer::_cube[VOXEL_VERTEX_COUNT];
    size_t   Renderer::_rend_verts;
    size_t   Renderer::_rend_faces;
    size_t   Renderer::_total_verts;
    size_t   Renderer::_total_faces;

    void Renderer::InitSkybox(Texture* sides, Texture* top, Texture* btm)
    {
        for (int i = 0; i < 4; i++) { skybox[i] = sides; }
        skybox[VOXELFACE_TOP] = top;
        skybox[VOXELFACE_BOTTOM] = btm;
    }

    void Renderer::Begin()
    {
        _rend_verts = 0;
        _rend_faces = 0;
        _total_verts = 0;
        _total_faces = 0;

        // Begin context
        GraphicsManager::device->BeginScene();

        // Configure texture stage properties
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

        // Configure render state
        GraphicsManager::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        GraphicsManager::device->SetRenderState(D3DRS_ZENABLE, TRUE);
    }

    void Renderer::End()
    {
        GraphicsManager::device->EndScene();
    }

    void Renderer::BeginFog(float start, float end, float density, D3DXCOLOR color)
    {
        D3DXVECTOR4 vFog(1.0f, 0.0f, 0.0f, 0.0f);
        GraphicsManager::device->SetVertexShaderConstant(44, &vFog, 1);

        D3DXCOLOR inverse_color;
        inverse_color.b = color.a;
        inverse_color.g = color.r;
        inverse_color.r = color.g;
        inverse_color.a = color.b;


        GraphicsManager::device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
        GraphicsManager::device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
        GraphicsManager::device->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));
        GraphicsManager::device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&density));
        GraphicsManager::device->SetRenderState(D3DRS_FOGCOLOR, *(DWORD*)(&inverse_color));
        GraphicsManager::device->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }

    void Renderer::EndFog()
    {
        GraphicsManager::device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }

    void Renderer::DrawPrimitive(D3DPRIMITIVETYPE type, const void* data, DWORD count)
    {
        GraphicsManager::device->DrawPrimitiveUP(type, count, data, sizeof(Vertex));
    }

    void Renderer::DrawCube(Texture** textures, const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, bool tile, bool cull, D3DXCOLOR color)
    {
        _rend_verts += 24;
        _rend_faces += 6;
        _total_verts += 24;
        _total_faces += 6;

        int   j = 0, k = 0;
        float tw = 1, th = 1;

        if (!cull) { GraphicsManager::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); }

        for (int f = 0; f < VOXEL_FACE_COUNT; f++)
        {
            if (tile)
            {
                if (f == VOXELFACE_FRONT || f == VOXELFACE_BACK) { tw = scale.x; th = scale.y; }
                else if (f == VOXELFACE_LEFT || f == VOXELFACE_RIGHT) { tw = scale.z; th = scale.y; }
                else if (f == VOXELFACE_BOTTOM) { tw = scale.x; th = scale.z; }
                else if (f == VOXELFACE_TOP) { tw = scale.z; th = scale.x; }
            }

            _cube[(f * FACE_VERTEX_COUNT) + 0].uv = VOXEL_UVS[(f * FACE_VERTEX_COUNT) + 0];
            _cube[(f * FACE_VERTEX_COUNT) + 1].uv = VOXEL_UVS[(f * FACE_VERTEX_COUNT) + 1];
            _cube[(f * FACE_VERTEX_COUNT) + 2].uv = VOXEL_UVS[(f * FACE_VERTEX_COUNT) + 2];
            _cube[(f * FACE_VERTEX_COUNT) + 3].uv = VOXEL_UVS[(f * FACE_VERTEX_COUNT) + 3];

            GraphicsManager::device->SetTexture(0, textures[f] == NULL ? NULL : textures[f]->D3D());

            if (cull)
            {
                if (f == VOXELFACE_FRONT || f == VOXELFACE_LEFT || f == VOXELFACE_TOP) { GraphicsManager::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); }
                else { GraphicsManager::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); }
            }

            for (int v = 0; v < FACE_VERTEX_COUNT; v++)
            {
                _cube[(f * FACE_VERTEX_COUNT) + v].color = color;
                _cube[(f * FACE_VERTEX_COUNT) + v].uv.x *= tw; _cube[(f * FACE_VERTEX_COUNT) + v].uv.y *= th;
                _cube[(f * FACE_VERTEX_COUNT) + v].pos = MathHelper::Multiply(VOXEL_VERTICES[(f * FACE_VERTEX_COUNT) + v], scale);
                _cube[(f * FACE_VERTEX_COUNT) + v].pos += pos;
            }

            Vertex* verts = (Vertex*)((UINT_PTR)_cube + (f * FACE_VERTEX_COUNT * sizeof(Vertex)));

            DrawPrimitive(D3DPT_QUADLIST, verts, 1);
        }
    }

    void Renderer::DrawPlane(Texture* texture, const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, bool tile, D3DXCOLOR color)
    {
        for (int v = 0; v < FACE_VERTEX_COUNT; v++)
        {
            _cube[v].pos = MathHelper::Multiply(VOXEL_VERTICES[FACE_VERTEX_COUNT * VOXELFACE_TOP], scale);
            _cube[v].uv = VOXEL_UVS[FACE_VERTEX_COUNT * VOXELFACE_TOP];
            if (tile) { _cube[v].uv.x *= scale.z; _cube[v].uv.y *= scale.x; }

            _cube[v].pos += pos;
            _cube[v].color = color;
        }

       // if (((_cube[0].color & 0xFF000000) >> 24) == 0)  { return; }

        GraphicsManager::device->SetTexture(0, texture->D3D());
        DrawPrimitive(D3DPT_QUADLIST, _cube, 1);
    }

    void Renderer::DrawSkybox(const D3DXVECTOR3& scale, D3DXCOLOR color)
    {
        D3DXVECTOR3 pos(-(scale.x / 2), -(scale.y / 2), -(scale.z / 2));

        DrawCube(skybox, pos, scale, false, false, color);
    }

    size_t Renderer::GetRenderedVertexCount() { return _rend_verts; }

    size_t Renderer::GetRenderedFaceCount() { return _rend_faces; }

    size_t Renderer::GetTotalVertexCount() { return _total_verts; }

    size_t Renderer::GetTotalFaceCount() { return _total_faces; }

}