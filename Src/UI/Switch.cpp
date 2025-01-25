#include "UI/Switch.h"
#include "Minecraft.h"

namespace Minecraft
{
    UI::Switch::Switch() : UI::Widget()
    {
    }

    UI::Switch::Switch(int x, int y, bool toggled, std::string label) : UI::Widget(x, y, 80, 40, label), toggled(toggled)
    {

    }

    void UI::Switch::Load()
    {
        UI::Widget::Load();
    }

    void UI::Switch::Unload()
    {
        UI::Widget::Unload();
    }

    void UI::Switch::Update(float dt)
    {
        UI::Widget::Update(dt);

        bounds.w = 80;
        bounds.h = 40;

        if (selected && !Input::flags[INPUTFLAG_BTN_A] && Input::IsBtnDown(0, GamepadButton::a))
        {
            toggled = !toggled;
            if (e_select != NULL) { e_select(this, NULL); }
            Input::flags[INPUTFLAG_BTN_A] = true;
        }
        if (Input::IsBtnUp(0, GamepadButton::a)) { Input::flags[INPUTFLAG_BTN_A] = false; }
    }

    void UI::Switch::Draw(float dt)
    {
        UI::Widget::Draw(dt);

        Texture* tex = &Assets::textures["SwitchOff"];
        if (toggled) { tex = &Assets::textures["SwitchOn"]; }

        D3DXCOLOR bg = D3DCOLOR_ARGB(0xFF, 0x7F, 0x7F, 0x7F);
        if (selected) { bg = D3DCOLOR_ARGB(255, 255, 255, 255); }

        Canvas::Draw(bounds.x, bounds.y, bounds.w, bounds.h, tex, bg);

        int th = Assets::font.sz.y + Assets::font.spacing.y;
        int y = bounds.y + ((bounds.h / 2) - (th / 2));

        Canvas::DrawStringShadow(bounds.x + 88, y, 1.0f, label.c_str(), D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 2, &Assets::font);

    }
}