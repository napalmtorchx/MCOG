#include "Graphics/GraphicsManager.h"
#include "Minecraft.h"

namespace Minecraft
{
    LPDIRECT3D8       GraphicsManager::engine;
    LPDIRECT3DDEVICE8 GraphicsManager::device;
    DWORD             GraphicsManager::shader;

    void GraphicsManager::Init()
    {
        // Create Direct3D engine instance
        engine = Direct3DCreate8(D3D_SDK_VERSION);
        if (engine == NULL) { RXDK_LOG_ERROR("Failed to create Direct3D engine"); return; }
        RXDK_LOG_INFO("Created Direct3D engine\n");

        // Initialize Direct3D device parameters structure
        D3DPRESENT_PARAMETERS params;
        ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));

        // Setup device parameters
        params.BackBufferWidth = 640;
        params.BackBufferHeight = 480;
        params.BackBufferFormat = D3DFMT_X8R8G8B8;
        params.BackBufferCount = 1;
        params.EnableAutoDepthStencil = true;
        params.AutoDepthStencilFormat = D3DFMT_D24S8;
        params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        params.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        // Create Direct3D device instance
        HRESULT res = engine->CreateDevice(0, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device);
        if (FAILED(res)) { RXDK_LOG_ERROR("Failed to create Direct3D device"); return; }
        RXDK_LOG_INFO("Created Direct3D device - Size:%dx%d\n", params.BackBufferWidth, params.BackBufferHeight);

        // Initialize shader
        InitShader();
        RXDK_LOG_INFO("Initialized graphics manager\n");
    }

    void GraphicsManager::InitShader()
    {
        // Open vertex shader file
        HANDLE fp = CreateFileA(VERTEX_SHADER_PATH, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (fp == INVALID_HANDLE_VALUE) { RXDK_LOG_ERROR("Failed to locate vertex shader at '%s'", VERTEX_SHADER_PATH); return; }
        RXDK_LOG_INFO("Opened vertex shader at '%s'\n", VERTEX_SHADER_PATH);

        // Attempt to read file contents into array
        DWORD sz = GetFileSize(fp, NULL), sz_read = 0;
        BYTE* data = new BYTE[sz];
        if (!ReadFile(fp, data, sz, &sz_read, NULL) || sz_read != sz)
        {
            CloseHandle(fp);
            delete[] data;
            RXDK_LOG_ERROR("Failed to load vertex shader at '%s'", VERTEX_SHADER_PATH);
            return;
        }
        // Successfully read file contents
        CloseHandle(fp);
        RXDK_LOG_INFO("Loaded vertex shader - Size:%d bytes File:'%s'\n", sz, VERTEX_SHADER_PATH);

        // Create vertex shader object
        shader = NULL;
        HRESULT res = device->CreateVertexShader(VERTEX_SHADER_DECL, (DWORD*)data, &shader, 0);
        if (FAILED(res)) { RXDK_LOG_ERROR("Error creating vertex shader"); }
        delete[] data;

        // Finished creating vertex shader
        RXDK_LOG_INFO("Successfully created vertex shader\n");
    }

    // Dispose graphics manager and Direct3D objects 
    void GraphicsManager::Dispose()
    {
        RXDK_LOG_INFO("Disposed graphics manager\n");
    }

    void GraphicsManager::EnableShader()
    {
        device->SetVertexShader(shader);
    }

    void GraphicsManager::DisableShader()
    {
        device->SetVertexShader(NULL);
    }

    // Clear the screen with a specified color
    void GraphicsManager::Clear(DWORD flags, D3DXCOLOR color)
    {
        device->Clear(0, NULL, flags, color, 1.0f, 0);
    }

    // Present rendering context to graphics device
    void GraphicsManager::Present()
    {
        device->Present(NULL, NULL, NULL, NULL);
    }
}