#include "Engine/Entity.h"
#include "Minecraft.h"

namespace Minecraft
{
    Entity::Entity() : Component(), pos(0, 0, 0)
    {

    }

    Entity::Entity(LPCSTR name, float x, float y, float z) : Component(name), pos(x, y, z)
    {

    }

    void Entity::Load()
    {
        Component::Load();
    }

    void Entity::Unload()
    {
        Component::Unload();
    }

    void Entity::Update(float dt)
    {
        Component::Update(dt);
    }

    void Entity::UpdateFixed()
    {
        Component::UpdateFixed();
    }

    void Entity::Draw(float dt)
    {
        Component::Draw(dt);
    }
}