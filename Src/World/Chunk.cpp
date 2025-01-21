#include "World/Chunk.h"
#include "Minecraft.h"

namespace Minecraft
{
	Chunk::Chunk() : Component("chunk"), blocks(NULL), pos(-1, -1, -1), index(-1), loaded(false)
	{

	}

	Chunk::Chunk(int x, int y, int z) : Component("chunk"), blocks(NULL), pos(x, y, z), index(-1), loaded(false)
	{

	}

	void Chunk::Load()
	{
		Component::Load();
		blocks = new Block[CHUNK_SIZE];
		Fill(1);
		mesh = ChunkMesh(this);
		mesh.Generate();
		loaded = true;
	}

	void Chunk::Unload()
	{
		Component::Unload();
		if (blocks != NULL) { delete[] blocks; blocks = NULL; }
		mesh.faces.clear();
		mesh.facemap.clear();
		pos   = D3DXVECTOR3(-1, -1, -1);
		index = -1;
	}

	void Chunk::Update(float dt)
	{
		Component::Update(dt);
	}

	void Chunk::UpdateFixed()
	{
		Component::UpdateFixed();
	}

	void Chunk::Draw(float dt)
	{
		Component::Draw(dt);
		if (!mesh.generated) { return; }

		D3DXVECTOR3 wpos = MathHelper::Multiply(pos, D3DXVECTOR3((float)CHUNK_WIDTH, (float)CHUNK_HEIGHT, (float)CHUNK_DEPTH));

		size_t rf = 0;

		for (DWORD i = 0; i < mesh.faces.size(); i++)
		{
			ChunkMeshFace& face = mesh.faces[i];

			D3DXVECTOR3 lazypos = face.vertices[0].pos;
			float    dist = MathHelper::Distance(lazypos,GameState::instance->player.pos);
			if (dist >= 128.0f) { continue; }

			GraphicsManager::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			if (face.face == VOXELFACE_FRONT || face.face == VOXELFACE_LEFT || face.face == VOXELFACE_TOP) { GraphicsManager::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); }
			GraphicsManager::device->SetTexture(0, face.texture == NULL ? NULL : face.texture->D3D());
			Renderer::DrawPrimitive(D3DPT_QUADLIST, face.vertices, 1);
			rf++;
		}

		_rend_faces = rf;
		_total_faces = mesh.faces.size();
	}

	void Chunk::Fill(BlockID id)
	{
		DWORD i = 0;
		while (i < CHUNK_SIZE) { blocks[i++] = Block(id); }
	}

	void Chunk::Set(int x, int y, int z, BlockID id)
	{
		int i = BLOCK_INDEX(x, y, z);
		if ((DWORD)i >= (DWORD)CHUNK_SIZE) { return; }
		blocks[i] = Block(id);
	}

	Block* Chunk::Get(int x, int y, int z)
	{
		int i = BLOCK_INDEX(x, y, z);
		if ((DWORD)i >= (DWORD)CHUNK_SIZE) { return NULL; }
		return &blocks[i];
	}

	size_t Chunk::GetRenderedFaceCount() { return _rend_faces; }

	size_t Chunk::GetTotalFaceCount() { return _total_faces; }
}