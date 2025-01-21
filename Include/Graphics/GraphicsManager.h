#pragma once
#include "Common.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define VERTEX_SHADER_PATH "D:\\Assets\\Shaders\\shader.xvu"

namespace Minecraft
{
    class GraphicsManager
    {
        public:
            static LPDIRECT3D8       engine;
            static LPDIRECT3DDEVICE8 device;
            static DWORD             shader;

        public:
            static void Init();
            static void InitShader();
            static void Dispose();

        public:
            static void EnableShader();
            static void DisableShader();

        public:
            static void Clear(DWORD flags, D3DXCOLOR color);
            static void Present();
    };
}