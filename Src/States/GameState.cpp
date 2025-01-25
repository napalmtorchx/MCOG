#include "States/GameState.h"
#include "Minecraft.h"

namespace Minecraft
{
    GameState* GameState::instance = NULL;

    GameState::GameState() : State("game"), world(NULL)
    {

    }

    void GameState::Load()
    {
        State::Load();

        player = Player("player", WORLD_WIDTH / 2, WORLD_HEIGHT + 2, WORLD_DEPTH / 2);
        player.Load();
    }

    void GameState::Unload()
    {
        State::Unload();
    }

    void GameState::Update(float dt)
    {
        State::Update(dt);
        world->Update(dt);
        player.Update(dt);
    }

    void GameState::UpdateFixed()
    {
        State::UpdateFixed();
        world->UpdateFixed();
        player.UpdateFixed();
    }

    void GameState::Draw(float dt)
    {
        State::Draw(dt);

        GraphicsManager::Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0xFF, 0x6E, 0xB1, 0xFF));
        GraphicsManager::EnableShader();

        player.Draw(dt);

        Renderer::Begin();

        Renderer::DrawSkybox(D3DXVECTOR3(3072.0f, 3072.0f, 3072.0f), D3DCOLOR_ARGB(255, 255, 255, 255));
        world->Draw(dt);
        if (player.placement_pos.x != -99999) { Renderer::DrawCube(NULL, player.placement_pos, D3DXVECTOR3(1, 1, 1), false, true, D3DCOLOR_ARGB(100, 255, 128, 128)); }

        Renderer::End();
    }

    void GameState::DrawUI(float dt)
    {
        State::DrawUI(dt);

        Canvas::Begin();
        Canvas::Draw(Rectangle((SCREEN_WIDTH / 2) - 64, (SCREEN_HEIGHT / 2) - 64, 128, 128), &Assets::textures["Crosshair"], D3DCOLOR_ARGB(255, 255, 255, 255));

        Canvas::DrawStringShadow(4, 4, 1.0f, App::GetDebugText(), D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 2, &Assets::font);
        Canvas::End();
    }
}