#include "Engine/Component.h"
#include "Minecraft.h"

namespace Minecraft
{
    Component::Component()
    {

    }

    Component::Component(LPCSTR name) : _name(name)
    {

    }

    void Component::Load()
    {

    }

    void Component::Unload()
    {

    }

    void Component::Update(float dt)
    {

    }

    void Component::UpdateFixed()
    {

    }

    void Component::Draw(float dt)
    {

    }

    std::string Component::Name() { return _name; }
}