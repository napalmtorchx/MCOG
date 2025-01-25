#pragma once
#include "UI/Widget.h"

#define BUTTON_NORMAL 0
#define BUTTON_LARGE  1

namespace Minecraft
{
    namespace UI
    {
        class Switch : public Widget
        {
            public: 
                bool toggled;

            public:
                Switch();
                Switch(int x, int y, bool toggled, std::string label);
                void Load() override;
                void Unload() override;
                void Update(float dt) override;
                void Draw(float dt) override;
        };
    }
}