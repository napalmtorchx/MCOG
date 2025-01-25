#pragma once
#include "Common.h"
#include "Application.h"

// Engine
#include "Engine/Component.h"
#include "Engine/Debug.h"
#include "Engine/State.h"
#include "Engine/Assets.h"
#include "Engine/Input.h"
#include "Engine/Entity.h"
#include "Engine/Options.h"

// Graphics
#include "Graphics/Ray.h"
#include "Graphics/Rectangle.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureAtlas.h"
#include "Graphics/Vertex.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/SpriteFont.h"
#include "Graphics/Frustum.h"
#include "Graphics/Canvas.h"
#include "Graphics/Voxel.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"

// Entities
#include "Entities/Player.h"

// States
#include "States/GameState.h"
#include "States/MenuState.h"

// World
#include "World/Block.h"
#include "World/Chunk.h"
#include "World/ChunkMesh.h"
#include "World/World.h"
#include "World/WorldGenerator.h"

// UI
#include "UI/Widget.h"
#include "UI/Button.h"
#include "UI/Switch.h"

// Util
#include "Util/MathHelper.h"
#include "Util/FileSystem.h"
#include "Util/IniParser.h"
#include "Util/FastNoiseLite.h"
#include "Util/BoundingBox.h"
#include "Util/Collision.h"