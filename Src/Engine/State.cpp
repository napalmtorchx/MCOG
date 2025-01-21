#include "Engine/State.h"
#include "Minecraft.h"

namespace Minecraft
{
    State::State() : Component()
    {
    }

    State::State(LPCSTR name) : Component(name)
    {
    }

    void State::Load()
    {
        Component::Load();
    }

    void State::Unload()
    {
        Component::Unload();
    }

    void State::Update(float dt)
    {
        Component::Update(dt);
    }

    void State::UpdateFixed()
    {
        Component::UpdateFixed();
    }

    void State::Draw(float dt)
    {
        Component::Draw(dt);
    }

    void State::DrawUI(float dt)
    {

    }

    // ----------------------------------------------------------------------------------------------------------------

    std::vector<State*> States::states;
    int                 States::index;

    void States::Init()
    {
        states.clear();
        index = -1;

        GameState::instance = new GameState();
        Load(GameState::instance);
        Set(GameState::instance);

        RXDK_LOG_INFO("Initialized state manager\n");
    }

    void States::Update(float dt)
    {
        if (index < 0 || index >= (int)states.size()) { return; }
        states[index]->Update(dt);
    }

    void States::UpdateFixed()
    {
        if (index < 0 || index >= (int)states.size()) { return; }
        states[index]->UpdateFixed();
    }

    void States::Draw(float dt)
    {
        if (index < 0 || index >= (int)states.size()) { return; }
        states[index]->Draw(dt);
    }

    void States::DrawUI(float dt)
    {
        if (index < 0 || index >= (int)states.size()) { return; }
        states[index]->DrawUI(dt);
    }

    void States::Load(State* state)
    {
        if (state == NULL) { RXDK_LOG_ERROR("Null state pointer"); return; }
        states.push_back(state);
        state->Load();
        RXDK_LOG_INFO("Loaded state %s(%p)\n", state->Name().c_str(), state);
    }

    void States::Unload(State* state)
    {
        if (state == NULL) { RXDK_LOG_ERROR("Null state pointer"); return; }
        for (size_t i = 0; i < states.size(); i++)
        {
            if (states[i] == state)
            {
                states.erase(states.begin() + i);
                state->Unload();
                RXDK_LOG_INFO("Unloaded state %s(%p)\n", state->Name().c_str(), state);
                return;
            }
        }
        RXDK_LOG_ERROR("Failed to unload state %s(%p)", state->Name().c_str(), state);
    }

    void States::Set(State* state) { Set(state->Name()); }

    void States::Set(const std::string& name)
    {
        for (size_t i = 0; i < states.size(); i++)
        {
            if (states[i]->Name() == name) 
            { 
                RXDK_LOG_INFO("Set state to %s(%p)\n", states[i]->Name().c_str(), states[i]);
                index = (int)i; 
                return; 
            }
        }
        RXDK_LOG_ERROR("Failed to locate state with name '%s'", name.c_str());
    }

    State* States::GetActive()
    {
        if (index < 0 || index >= (int)states.size()) { return NULL; }
        return states[index];
    }

    State* States::Get(const std::string& name)
    {
        for (size_t i = 0; i < states.size(); i++)
        {
            if (states[i]->Name() == name) { return states[i]; }
        }
        return NULL;
    }
}