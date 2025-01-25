#pragma once
#include "Common.h"
#include "Graphics/Rectangle.h"

namespace Minecraft
{
	namespace UI
	{
		class Widget;

		typedef void (*WidgetEvent)(Widget* widget, void* arg);

		class Widget
		{
            public:
                std::string label;
                Rectangle   bounds;
                bool        selected;

            public:
                WidgetEvent e_select;

            public:
                Widget();
                Widget(int x, int y, int w, int h, std::string label);
                virtual void Load();
                virtual void Unload();
                virtual void Update(float dt);
                virtual void Draw(float dt);
		};
	}
}