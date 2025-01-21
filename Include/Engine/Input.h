#pragma once
#include "Common.h"

#define INPUTFLAG_DPAD_UP    0
#define INPUTFLAG_DPAD_DOWN  1
#define INPUTFLAG_DPAD_LEFT  2
#define INPUTFLAG_DPAD_RIGHT 3
#define INPUTFLAG_BTN_Y      4
#define INPUTFLAG_BTN_X      5
#define INPUTFLAG_BTN_B      6
#define INPUTFLAG_BTN_A      7 

namespace Minecraft
{
    namespace GamepadButton
    {
        static const DWORD dpad_up = 0x00000001;
        static const DWORD dpad_down = 0x00000002;
        static const DWORD dpad_left = 0x00000004;
        static const DWORD dpad_right = 0x00000008;
        static const DWORD start = 0x00000010;
        static const DWORD back = 0x00000020;
        static const DWORD thumb_l = 0x00000040;
        static const DWORD thumb_r = 0x00000080;
        static const DWORD a = 0x00000100;
        static const DWORD b = 0x00000101;
        static const DWORD x = 0x00000102;
        static const DWORD y = 0x00000103;
        static const DWORD black = 0x00000104;
        static const DWORD white = 0x00000105;
    }

    class Input
    {
    public:
        static HANDLE gp_handle;
        static bool   flags[];

    private:
        static XINPUT_GAMEPAD _gp;

    public:
        static void Init();
        static void Update(float dt);

    public:
        static bool     IsBtnDown(DWORD gpid, DWORD btn);
        static bool     IsBtnUp(DWORD gpid, DWORD btn);
        static D3DXVECTOR2 GetThumbL(DWORD gpid);
        static D3DXVECTOR2 GetThumbR(DWORD gpid);
        static float    GetTriggerL(DWORD gpid);
        static float    GetTriggerR(DWORD gpid);
    };
}