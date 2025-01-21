#pragma once
#include "Engine/Entity.h"
#include "Graphics/Camera.h"

namespace Minecraft
{
    class Player : public Entity
    {
        public:
            Camera cam;
        
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