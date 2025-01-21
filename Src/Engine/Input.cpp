#include "Engine/Input.h"
#include "Minecraft.h"


namespace Minecraft
{
    HANDLE         Input::gp_handle;
    bool           Input::flags[16];
    XINPUT_GAMEPAD Input::_gp;

    void Input::Init()
    {
        // Initialize input device controller
        XDEVICE_PREALLOC_TYPE dev_types[] = { { XDEVICE_TYPE_GAMEPAD, 1 } };
        XInitDevices(sizeof(dev_types) / sizeof(XDEVICE_PREALLOC_TYPE), dev_types);
        Sleep(1000);

        // Check for conntected devices
        DWORD connected = XGetDevices(XDEVICE_TYPE_GAMEPAD);
        if (connected)
        {
            gp_handle = XInputOpen(XDEVICE_TYPE_GAMEPAD, XDEVICE_PORT0, XDEVICE_NO_SLOT, NULL);
            if (gp_handle == NULL) { RXDK_LOG_ERROR("Failed to initialize gamepad 1"); return; }
            else { RXDK_LOG_INFO("Gamepad 1 connected - Handle:%p\n", gp_handle); }
        }
        else { RXDK_LOG_WARN("Gamepad 1 NOT connected\n"); }

        memset(flags, 0, sizeof(flags));

        // Finished initializing input manager
        RXDK_LOG_INFO("Initialized input manager\n");
    }

    void Input::Update(float dt)
    {
        XINPUT_STATE xis;
        XInputGetState(gp_handle, &xis);
        _gp = xis.Gamepad;

        if (IsBtnUp(0, GamepadButton::dpad_up)) { flags[INPUTFLAG_DPAD_UP] = false; }
        if (IsBtnUp(0, GamepadButton::dpad_down)) { flags[INPUTFLAG_DPAD_DOWN] = false; }
        if (IsBtnUp(0, GamepadButton::dpad_left)) { flags[INPUTFLAG_DPAD_LEFT] = false; }
        if (IsBtnUp(0, GamepadButton::dpad_right)) { flags[INPUTFLAG_DPAD_RIGHT] = false; }
        if (IsBtnUp(0, GamepadButton::a)) { flags[INPUTFLAG_BTN_A] = false; }
        if (IsBtnUp(0, GamepadButton::b)) { flags[INPUTFLAG_BTN_B] = false; }
        if (IsBtnUp(0, GamepadButton::x)) { flags[INPUTFLAG_BTN_X] = false; }
        if (IsBtnUp(0, GamepadButton::y)) { flags[INPUTFLAG_BTN_Y] = false; }
    }

    bool Input::IsBtnDown(DWORD gpid, DWORD btn)
    {
        if ((WORD)btn < 0x0100) { return (_gp.wButtons & (WORD)btn) != 0; }
        return _gp.bAnalogButtons[(DWORD)btn - 0x0100] != 0;
    }

    bool Input::IsBtnUp(DWORD gpid, DWORD btn) { return !IsBtnDown(gpid, btn); }

    D3DXVECTOR2 Input::GetThumbL(DWORD gpid)
    {
        float x = (float)_gp.sThumbLX / 32768.0f;
        float y = (float)_gp.sThumbLY / 32768.0f;
        return D3DXVECTOR2(x, y);
    }

    D3DXVECTOR2 Input::GetThumbR(DWORD gpid)
    {
        float x = (float)_gp.sThumbRX / 32768.0f;
        float y = (float)_gp.sThumbRY / 32768.0f;
        return D3DXVECTOR2(x, y);
    }

    float Input::GetTriggerL(DWORD gpid)
    {
        return (float)_gp.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] / 255.0f;
    }

    float Input::GetTriggerR(DWORD gpid)
    {
        return (float)_gp.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] / 255.0f;
    }
}