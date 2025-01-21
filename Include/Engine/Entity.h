#pragma once
#include "Engine/Component.h"

namespace Minecraft
{
    class Entity : public Component
    {
        public:
            D3DXVECTOR3 pos;
            bool        rendered;

        public:
            Entity();
            Entity(LPCSTR name, float x, float y, float z);
            void Load() override;
            void Unload() override;
            void Update(float dt) override;
            void UpdateFixed() override;
            void Draw(float dt) override;
    };
}