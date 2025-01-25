#pragma once
#include "Engine/Entity.h"
#include "Graphics/Camera.h"
#include "Graphics/Frustum.h"

namespace Minecraft
{
    class Chunk;

    class Player : public Entity
    {
        public:
            Camera      cam;
            Frustum     frustum;
            D3DXVECTOR3 hitpos;
            Chunk*      hitchunk;
            D3DXVECTOR3 speed;
            D3DXVECTOR3 placement_pos;
            bool        placing, breaking, jumping, jmpdwn;
        
        public:
            Player();
            Player(LPCSTR name, float x, float y, float z);
            void Load() override;
            void Unload() override;
            void Update(float dt) override;
            void UpdateFixed() override;
            void Draw(float dt) override;
    };
}