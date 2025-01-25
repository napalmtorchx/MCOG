#pragma once
#include "Engine/State.h"
#include "UI/Button.h"
#include "UI/Switch.h"

namespace Minecraft
{
    class MenuState : public State
    {
        public:
            static MenuState* instance;

        public:
            static const int btn_play = 0;
            static const int btn_opt = 1;
            static const int btn_exit = 2;
            static const int button_count = 3;

        public:
            UI::Button buttons[button_count];
            int        index;

        public:
            MenuState();
            void Load() override;
            void Unload() override;
            void Update(float dt) override;
            void UpdateFixed() override;
            void Draw(float dt) override;
            void DrawUI(float dt) override;

        private:
            static void OnBtnPlayPressed(UI::Widget* sender, void* arg);
            static void OnBtnOptPressed(UI::Widget* sender, void* arg);
            static void OnBtnExitPressed(UI::Widget* sender, void* arg);
    };
}