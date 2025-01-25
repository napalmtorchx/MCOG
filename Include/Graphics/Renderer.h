#pragma once
#include "Common.h"
#include "Graphics/Rectangle.h"
#include "Graphics/Texture.h"
#include "Graphics/Vertex.h"

namespace Minecraft
{
    class Renderer
    {
        public:
            static Texture* skybox[];

        private:
            static Vertex _cube[];
            static size_t _rend_verts, _rend_faces, _total_verts, _total_faces;

        public:
            static void InitSkybox(Texture** sides, Texture* top, Texture* btm);
            static void Begin();
            static void End();

        public:
            static void BeginFog(float start, float end, float density, D3DXCOLOR color);
            static void EndFog();
            static void DrawPrimitive(D3DPRIMITIVETYPE type, const void* data, DWORD count);
            static void DrawCube(Texture** textures, const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, bool tile, bool cull, D3DXCOLOR color);
            static void DrawPlane(Texture* texture, const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, bool tile, D3DXCOLOR color);
            static void DrawSkybox(const D3DXVECTOR3& scale, D3DXCOLOR color);

        public:
            static size_t GetRenderedVertexCount();
            static size_t GetRenderedFaceCount();
            static size_t GetTotalVertexCount();
            static size_t GetTotalFaceCount();
    };
}