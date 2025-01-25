#pragma once
#include "UI/Widget.h"

#define BUTTON_NORMAL 0
#define BUTTON_LARGE  1

namespace Minecraft
{
    namespace UI
    {
        class Button : public Widget
        {
            public:
                int type;

            public:
                Button();
                Button(int x, int y, int type, std::string label);
                void Load() override;
                void Unload() override;
                void Update(float dt) override;
                void Draw(float dt) override;
        };
    }
}