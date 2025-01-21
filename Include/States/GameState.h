#pragma once
#include "Engine/State.h"
#include "Graphics/Camera.h"
#include "World/World.h"
#include "Entities/Player.h"

namespace Minecraft
{
	class GameState : public State
	{
        public:
            static GameState* instance;

        public:
            World* world;
            Player player;

		public:
            GameState();
            void Load() override;
            void Unload() override;
            void Update(float dt) override;
            void UpdateFixed() override;
            void Draw(float dt) override;
            void DrawUI(float dt) override;
	};
}