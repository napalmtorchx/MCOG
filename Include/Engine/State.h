#pragma once
#include "Component.h"

namespace Minecraft
{
    class State : public Component
    {
        public:
            State();
            State(LPCSTR name);
            void Load() override;
            void Unload() override;
            void Update(float dt) override;
            void UpdateFixed() override;
            void Draw(float dt) override;
            virtual void DrawUI(float dt);
    };

    class States
    {
        public:
            static std::vector<State*> states;
            static int                 index;

        public:
            static void Init();
            static void Update(float dt);
            static void UpdateFixed();
            static void Draw(float dt);
            static void DrawUI(float dt);
            static void Load(State* state);
            static void Unload(State* state);
            static void Set(State* state);
            static void Set(const std::string& name);
            static State* GetActive();
            static State* Get(const std::string& name);
    };
}