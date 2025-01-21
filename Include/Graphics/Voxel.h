#pragma once
#include "Common.h"

#define VOXEL_FACE_COUNT   6
#define FACE_VERTEX_COUNT  4
#define VOXEL_VERTEX_COUNT (VOXEL_FACE_COUNT * FACE_VERTEX_COUNT)

namespace Minecraft
{
    enum VOXELFACE
    {
        VOXELFACE_FRONT = 0,
        VOXELFACE_RIGHT = 1,
        VOXELFACE_BACK = 2,
        VOXELFACE_LEFT = 3,
        VOXELFACE_TOP = 4,
        VOXELFACE_BOTTOM = 5,
    };

    static const D3DXVECTOR3 VOXEL_VERTICES[VOXEL_VERTEX_COUNT] =
    {
        // Front face
        D3DXVECTOR3(0.0f, 1.0f, 0.0f),
        D3DXVECTOR3(1.0f, 1.0f, 0.0f),
        D3DXVECTOR3(1.0f, 0.0f, 0.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        // Right face
        D3DXVECTOR3(1.0f, 0.0f, 1.0f),
        D3DXVECTOR3(1.0f, 1.0f, 1.0f),
        D3DXVECTOR3(1.0f, 1.0f, 0.0f),
        D3DXVECTOR3(1.0f, 0.0f, 0.0f),
        // Back face
        D3DXVECTOR3(0.0f, 1.0f, 1.0f),
        D3DXVECTOR3(1.0f, 1.0f, 1.0f),
        D3DXVECTOR3(1.0f, 0.0f, 1.0f),
        D3DXVECTOR3(0.0f, 0.0f, 1.0f),
        // Left face
        D3DXVECTOR3(0.0f, 0.0f, 1.0f),
        D3DXVECTOR3(0.0f, 1.0f, 1.0f),
        D3DXVECTOR3(0.0f, 1.0f, 0.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        // Top face
        D3DXVECTOR3(0.0f, 1.0f, 1.0f),
        D3DXVECTOR3(1.0f, 1.0f, 1.0f),
        D3DXVECTOR3(1.0f, 1.0f, 0.0f),
        D3DXVECTOR3(0.0f, 1.0f, 0.0f),
        // Bottom face
        D3DXVECTOR3(0.0f, 0.0f, 1.0f),
        D3DXVECTOR3(1.0f, 0.0f, 1.0f),
        D3DXVECTOR3(1.0f, 0.0f, 0.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
    };

    static const D3DXVECTOR2 VOXEL_UVS[VOXEL_VERTEX_COUNT] =
    {
        // Front face
        D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR2(1.0f, 0.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        D3DXVECTOR2(0.0f, 1.0f),
        // Right face
        D3DXVECTOR2(1.0f, 1.0f),
        D3DXVECTOR2(1.0f, 0.0f),
        D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR2(0.0f, 1.0f),
        // Back face
        D3DXVECTOR2(1.0f, 0.0f),
        D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR2(0.0f, 1.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        // Left face
        D3DXVECTOR2(0.0f, 1.0f),
        D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR2(1.0f, 0.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        // Top face
        D3DXVECTOR2(1.0f, 0.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        D3DXVECTOR2(0.0f, 1.0f),
        D3DXVECTOR2(0.0f, 0.0f),
        // Bottom face
        D3DXVECTOR2(1.0f, 0.0f),
        D3DXVECTOR2(1.0f, 1.0f),
        D3DXVECTOR2(0.0f, 1.0f),
        D3DXVECTOR2(0.0f, 0.0f),
    };

    static const int VOXEL_LIGHTS[VOXEL_VERTEX_COUNT] =
    {
        // Front face
        224, 224, 224, 224,
        // Right face
        204, 204, 204, 204,
        // Back face
        102, 102, 102, 102,
        // Left face
        152, 152, 152, 152,
        // Top face
        255, 255, 255, 255,
        // Bottom face
        88,  88,  88,  88,
    };
}