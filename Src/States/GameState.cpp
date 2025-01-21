#include "States/GameState.h"
#include "Minecraft.h"

namespace Minecraft
{
    GameState* GameState::instance = NULL;

    GameState::GameState() : State("game")
    {

    }

    void GameState::Load()
    {
        State::Load();

        world = new World("TestWorld");
        world->Load();

        player = Player("player", 0, 0, -4);
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

        BlockMetadata* blk = &Assets::blocks[1];
        Renderer::Begin();

        world->Draw(dt);

        Renderer::End();
    }

    void GameState::DrawUI(float dt)
    {
        State::DrawUI(dt);

        Canvas::Begin();
        Canvas::DrawStringShadow(64, 60, 1.0f, App::GetDebugText(), D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 0, 0), 2, &Assets::font);
        Canvas::End();
    }
}