#include "Entities/Player.h"
#include "Minecraft.h"

float roundf(float x) {
    if (x >= 0.0f) {
        return (float)((int)(x + 0.5f));  // Round positive numbers
    }
    else {
        return (float)((int)(x - 0.5f));  // Round negative numbers
    }
}

namespace Minecraft
{
    Player::Player() : Entity(), hitpos(-99999, -99999, -99999), hitchunk(NULL)
    {

    }

    Player::Player(LPCSTR name, float x, float y, float z) : Entity(name, x, y, z), hitpos(-99999, -99999, -99999), hitchunk(NULL)
    {

    }

    void Player::Load()
    {
        Entity::Load();

        cam = Camera(D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(0, 0), 75.0f, 0.1f, 3072.0f);
        speed = D3DXVECTOR3(0, 0, 0);
        jumping = false;
    }

    void Player::Unload()
    {
        Entity::Unload();
    }

    void Player::Update(float dt)
    {
        Entity::Update(dt);

        if (hitpos.x == -99999) { placement_pos = D3DXVECTOR3(-99999, -99999, -99999); }
        else
        {
            Ray ray = Ray(cam.pos, D3DXVECTOR3(cosf(cam.lookdir.x), cam.lookdir.y, sinf(cam.lookdir.x)));
            D3DXVec3Normalize(&ray.direction, &ray.direction);

            placement_pos = hitpos;

            if (fabs(ray.direction.x) > fabs(ray.direction.y) && fabs(ray.direction.x) > fabs(ray.direction.z))
            {
                if (ray.direction.x > 0) { placement_pos.x -= 1; } // works
                else { placement_pos.x += 1; } // works
            }
            else if (fabs(ray.direction.y) > fabs(ray.direction.x) && fabs(ray.direction.y) > fabs(ray.direction.z))
            {
                if (ray.direction.y > 0) { placement_pos.y -= 1; } // broken
                else { placement_pos.y += 1; } // works
            }
            else  if (fabs(ray.direction.z) > fabs(ray.direction.y) && fabs(ray.direction.z) > fabs(ray.direction.x))
            {
                if (ray.direction.z > 0) { placement_pos.z -= 1; } // broken
                else { placement_pos.z += 1; } // works
            }
        }

        float movespd = 5.0f;
        float lookspd = 3.0f;
        float gravity = -9.8f;  // Gravity strength (negative value to pull down)
        float gravityMultiplier = 1.0f;  // Adjust the gravity effect if needed
        float jmpforce = 74.0f;

        // Get gamepad input values
        D3DXVECTOR2 v_tl = Input::GetThumbL(0), v_tr = Input::GetThumbR(0);
        bool btn_jmp = Input::IsBtnDown(0, GamepadButton::a) && !Input::flags[INPUTFLAG_BTN_A];
        bool btn_crouch = Input::IsBtnDown(0, GamepadButton::x) && !Input::flags[INPUTFLAG_BTN_B];

        if (Input::GetTriggerR(0) >= 0.95f && hitchunk != NULL && hitpos.x != -99999 && !breaking)
        {
            if (GameState::instance->world->GetBlock((int)hitpos.x, (int)hitpos.y, (int)hitpos.z, NULL)->id != 0)
            {
                GameState::instance->world->SetBlock((int)hitpos.x, (int)hitpos.y, (int)hitpos.z, 0);
                hitchunk->mesh.Generate();
            }
            breaking = true;
        }
        if (Input::GetTriggerR(0) == 0) { breaking = false; }

        if (Input::GetTriggerL(0) >= 0.95f && hitchunk != NULL && hitpos.x != -99999 && !placing)
        {
            if (GameState::instance->world->GetBlock((int)placement_pos.x, (int)placement_pos.y, (int)placement_pos.z, NULL)->id == 0)
            {
                GameState::instance->world->SetBlock((int)placement_pos.x, (int)placement_pos.y, (int)placement_pos.z, 3);
                hitchunk->mesh.Generate();
            }
            placing = true;
        }
        if (Input::GetTriggerL(0) == 0) { placing = false; }

        // Calculate forward and right movement vectors
        D3DXVECTOR3 v_forward = D3DXVECTOR3(cosf(cam.lookdir.x), 0.0f, sinf(cam.lookdir.x));
        D3DXVECTOR3 v_right;
        D3DXVec3Cross(&v_right, &v_forward, &cam.up);
        D3DXVec3Normalize(&v_forward, &v_forward);
        D3DXVec3Normalize(&v_right, &v_right);

        // Apply forward, backward, left, and right movement
        D3DXVECTOR3 mv(0, 0, 0);
        if (v_tl.x < -0.5f || v_tl.x > 0.5f) { v_right *= movespd * -v_tl.x * dt; mv += v_right; }
        if (v_tl.y < -0.5f || v_tl.y > 0.5f) { v_forward *= movespd * v_tl.y * dt; mv += v_forward; }
        speed = mv;

        // Apply vertical movement        
        if (btn_jmp && !jumping && !jmpdwn) 
        { 
            speed.y = jmpforce * dt;
            jumping = true;
            jmpdwn = true;
        }
        if (!btn_jmp) { jmpdwn = false; }

        // Apply view direction from thumbstick
        if (v_tr.x < -0.5f || v_tr.x > 0.5f) { cam.lookdir.x -= v_tr.x * lookspd * dt; }
        if (v_tr.y < -0.5f || v_tr.y > 0.5f) { cam.lookdir.y += v_tr.y * lookspd * dt; }

        // Clamp vertical viewing direction to 180 degrees
        float limit = D3DX_PI;

        if (cam.lookdir.y < -limit) { cam.lookdir.y = -limit; }
        if (cam.lookdir.y > limit) { cam.lookdir.y = limit; }

        D3DXVECTOR3 playerpos = D3DXVECTOR3(pos.x, pos.y - 1.0f, pos.z);
        D3DXVECTOR3 playersz = D3DXVECTOR3(0.9f, 1.0f, 0.9f);
        D3DXVECTOR3 blksz = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
        
        if (speed.y > -10.0f) {  // This helps prevent an excessive falling speed
            speed.y += gravity * gravityMultiplier * dt;  // Apply gravity force
        }

        for (int i = 0; i < WORLD_SIZE_CHUNKS; i++)
        {
            Chunk* c = &GameState::instance->world->chunks[i];
            if (c == NULL || c->dist_to_player >= 32.0f) { continue; }

            for (int i = 0; i < CHUNK_SIZE; i++)
            {
                if (c->blocks[i].id == 0) { continue; }
                int z = BLOCK_Z(i), y = BLOCK_Y(i, z), x = BLOCK_X(i, z, y);

                D3DXVECTOR3 blkpos = D3DXVECTOR3(x, y, z);
                blkpos += MathHelper::Multiply(c->pos, D3DXVECTOR3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH));

                if (speed.y < 0 && Collision::CheckBottom(playerpos, playersz, blkpos, blksz, speed.y))
                {
                    speed.y = 0;
                    pos.y = roundf(pos.y);
                    jumping = false;
                }
                if (speed.y > 0 && Collision::CheckTop(playerpos, playersz, blkpos, blksz, speed.y)) { speed.y = 0; pos.y = roundf(pos.y); }
                if (speed.x > 0 && Collision::CheckLeft(playerpos, playersz, blkpos, blksz, speed.x)) { speed.x = 0; pos.x = roundf(pos.x); }
                if (speed.x < 0 && Collision::CheckRight(playerpos, playersz, blkpos, blksz, speed.x)) { speed.x = 0; pos.x = roundf(pos.x); }
                if (speed.z < 0 && Collision::CheckBack(playerpos, playersz, blkpos, blksz, speed.z)) { speed.z = 0; pos.z = roundf(pos.z); }
                if (speed.z > 0 && Collision::CheckFront(playerpos, playersz, blkpos, blksz, speed.z)) { speed.z = 0; pos.z = roundf(pos.z); }
            }
        }

        // Update camera
        pos += speed;
        cam.pos = D3DXVECTOR3(pos.x + 0.5f, pos.y + 0.8f, pos.z + 0.5f);
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