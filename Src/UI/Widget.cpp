#include "UI/Widget.h"
#include "Minecraft.h"

namespace Minecraft
{
    UI::Widget::Widget()
    {
        this->bounds = Rectangle(0, 0, 0, 0);
        this->label = "";
        this->e_select = NULL;
        this->selected = false;
    }

    UI::Widget::Widget(int x, int y, int w, int h, std::string label)
    {
        this->bounds = Rectangle(x, y, w, h);
        this->label = label;
        this->e_select = NULL;
        this->selected = false;
    }

    void UI::Widget::Load()
    {

    }

    void UI::Widget::Unload()
    {

    }

    void UI::Widget::Update(float dt)
    {

    }

    void UI::Widget::Draw(float dt)
    {
    }
}