#include "UI/Button.h"
#include "Minecraft.h"

namespace Minecraft
{
    UI::Button::Button() : UI::Widget()
    {
        this->type = BUTTON_NORMAL;
    }

    UI::Button::Button(int x, int y, int type, std::string label) : UI::Widget(x, y, 256, 48, label)
    {
        this->type = type;
    }

    void UI::Button::Load()
    {
        UI::Widget::Load();
    }

    void UI::Button::Unload()
    {
        UI::Widget::Unload();
    }

    void UI::Button::Update(float dt)
    {
        UI::Widget::Update(dt);

        if (type == BUTTON_LARGE) { bounds.w = 256; bounds.h = 48; }
        else { bounds.w = 128; bounds.h = 48; }

        if (selected && !Input::flags[INPUTFLAG_BTN_A] && Input::IsBtnDown(0, GamepadButton::a))
        {
            if (e_select != NULL) { e_select(this, NULL); }
            Input::flags[INPUTFLAG_BTN_A] = true;
        }
        if (Input::IsBtnUp(0, GamepadButton::a)) { Input::flags[INPUTFLAG_BTN_A] = false; }
    }

    void UI::Button::Draw(float dt)
    {
        UI::Widget::Draw(dt);

        D3DXCOLOR bg = D3DCOLOR_ARGB(255, 255, 255, 255);
        if (selected) { bg.r = 0.5f; bg.g = 0.5f; }

        Canvas::Draw(bounds.x, bounds.y, bounds.w, bounds.h, &Assets::textures["ButtonLarge"], bg);

        int tw = ((int)Assets::font.sz.x + (int)Assets::font.spacing.x) * label.size();
        int th = (int)Assets::font.sz.y + (int)Assets::font.spacing.y;
        int x = bounds.x + ((bounds.w / 2) - (tw / 2));
        int y = bounds.y + ((bounds.h / 2) - (th / 2)) - 3;

        Canvas::DrawStringShadow(x, y, 1.0f, label.c_str(), D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 2, &Assets::font);
    }
}