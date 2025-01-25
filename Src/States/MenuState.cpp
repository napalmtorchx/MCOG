#include "States/MenuState.h"
#include "Minecraft.h"

namespace Minecraft
{
    MenuState* MenuState::instance = NULL;

    MenuState::MenuState() : State("menu")
    {
        int btn_y = (SCREEN_HEIGHT / 2) - 50;
        buttons[btn_play] = UI::Button((SCREEN_WIDTH / 2) - 128, btn_y, BUTTON_LARGE, "Play Game"); btn_y += 56;
        buttons[btn_play].e_select = OnBtnPlayPressed;
        buttons[btn_play].selected = true;

        buttons[btn_opt] = UI::Button((SCREEN_WIDTH / 2) - 128, btn_y, BUTTON_LARGE, "Help & Options"); btn_y += 56;
        buttons[btn_opt].e_select = OnBtnOptPressed;

        buttons[btn_exit] = UI::Button((SCREEN_WIDTH / 2) - 128, btn_y, BUTTON_LARGE, "Exit Game");
        buttons[btn_exit].e_select = OnBtnExitPressed;

        index = 0;
    }

    void MenuState::Load()
    {
        State::Load();
    }

    void MenuState::Unload()
    {
        State::Unload();
    }

    void MenuState::Update(float dt)
    {
        State::Update(dt);

        for (DWORD i = 0; i < button_count; i++)
        {
            buttons[i].selected = (i == index);
            buttons[i].Update(dt);
        }

        if (Input::IsBtnDown(0, GamepadButton::dpad_up) && !Input::flags[INPUTFLAG_DPAD_UP])
        {
            Input::flags[INPUTFLAG_DPAD_UP] = true;
            if (index > 0) { index--; }
        }
        if (Input::IsBtnUp(0, GamepadButton::dpad_up)) { Input::flags[INPUTFLAG_DPAD_UP] = false; }

        if (Input::IsBtnDown(0, GamepadButton::dpad_down) && !Input::flags[INPUTFLAG_DPAD_DOWN])
        {
            Input::flags[INPUTFLAG_DPAD_DOWN] = true;
            if (index < button_count - 1) { index++; }
        }
        if (Input::IsBtnUp(0, GamepadButton::dpad_down)) { Input::flags[INPUTFLAG_DPAD_DOWN] = false; }
    }

    void MenuState::UpdateFixed()
    {
        State::UpdateFixed();
    }

    void MenuState::Draw(float dt)
    {
        State::Draw(dt);
    }

    void MenuState::DrawUI(float dt)
    {
        State::DrawUI(dt);
        GraphicsManager::Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));

        Canvas::Begin();
        Canvas::DrawStringShadow(64, 60, 1.0f, "MINECRAFT XBOX EDITION\nversion 2.0", D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 2, &Assets::font);
        for (DWORD i = 0; i < button_count; i++) { buttons[i].Draw(dt); }
        Canvas::End();
    }

    void MenuState::OnBtnPlayPressed(UI::Widget* sender, void* arg)
    {
        GameState::instance->world = new World("TestWorld");
        GameState::instance->world->Load();
        States::Set(GameState::instance);
    }

    void MenuState::OnBtnOptPressed(UI::Widget* sender, void* arg)
    {

    }

    void MenuState::OnBtnExitPressed(UI::Widget* sender, void* arg)
    {

    }
}