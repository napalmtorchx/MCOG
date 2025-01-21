#pragma once
#include "Common.h"

namespace Minecraft
{
    class Component
    {
        protected:
            std::string _name;

        public:
            Component();
            Component(LPCSTR name);
            virtual void Load();
            virtual void Unload();
            virtual void Update(float dt);
            virtual void UpdateFixed();
            virtual void Draw(float dt);

        public:
            std::string Name();
    };
}