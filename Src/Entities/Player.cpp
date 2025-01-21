#include "Entities/Player.h"
#include "Minecraft.h"

namespace Minecraft
{
    Player::Player() : Entity()
    {

    }

    Player::Player(LPCSTR name, float x, float y, float z) : Entity(name, x, y, z)
    {

    }

    void Player::Load()
    {
        Entity::Load();

        cam = Camera(D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(0, 0), 75.0f, 0.1f, 2080.0f);
    }

    void Player::Unload()
    {
        Entity::Unload();
    }

    void Player::Update(float dt)
    {
        Entity::Update(dt);

        float movespd = 15.0f;
        float lookspd = 3.0f;

        // Get gamepad input values
        D3DXVECTOR2 v_tl = Input::GetThumbL(0), v_tr = Input::GetThumbR(0);
        bool btn_jmp = Input::IsBtnDown(0, GamepadButton::a) && !Input::flags[INPUTFLAG_BTN_A];
        bool btn_crouch = Input::IsBtnDown(0, GamepadButton::x) && !Input::flags[INPUTFLAG_BTN_B];

        // Calculate forward and right movement vectors
        D3DXVECTOR3 v_forward = D3DXVECTOR3(cosf(cam.lookdir.x), 0.0f, sinf(cam.lookdir.x));
        D3DXVECTOR3 v_right;
        D3DXVec3Cross(&v_right, &v_forward, &cam.up);
        D3DXVec3Normalize(&v_forward, &v_forward);
        D3DXVec3Normalize(&v_right, &v_right);

        // Apply forward, backward, left, and right movement
        if (v_tl.x < -0.5f || v_tl.x > 0.5f) { v_right *= movespd * -v_tl.x * dt; pos += v_right; }
        if (v_tl.y < -0.5f || v_tl.y > 0.5f) { v_forward *= movespd * v_tl.y * dt; pos += v_forward; }

        // Apply vertical movement        
        if (btn_jmp) { pos.y += movespd * dt; }
        if (btn_crouch) { pos.y -= movespd * dt; }

        // Apply view direction from thumbstick
        if (v_tr.x < -0.5f || v_tr.x > 0.5f) { cam.lookdir.x -= v_tr.x * lookspd * dt; }
        if (v_tr.y < -0.5f || v_tr.y > 0.5f) { cam.lookdir.y += v_tr.y * lookspd * dt; }

        // Clamp vertical viewing direction to 180 degrees
        float limit = D3DX_PI;

        if (cam.lookdir.y < -limit) { cam.lookdir.y = -limit; }
        if (cam.lookdir.y > limit) { cam.lookdir.y = limit; }

        // Update camera
        cam.pos = pos;
        cam.Update(dt);
    }

    void Player::UpdateFixed()
    {
        Entity::UpdateFixed();
    }

    void Player::Draw(float dt)
    {
        Entity::Draw(dt);
        cam.Project();
    }
}