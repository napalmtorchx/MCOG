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
		loaded = false;
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
		Player* player = &GameState::instance->player;
		D3DXVECTOR3 realpos = MathHelper::Multiply(pos, D3DXVECTOR3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH));

		contains_player = (player->pos.x >= realpos.x && player->pos.x < realpos.x + CHUNK_WIDTH &&
			player->pos.y >= realpos.y && player->pos.y < realpos.y + CHUNK_HEIGHT &&
			player->pos.z >= realpos.z && player->pos.z < realpos.z + CHUNK_DEPTH);


		D3DXVECTOR3 center = D3DXVECTOR3(pos.x * CHUNK_WIDTH, pos.y * CHUNK_HEIGHT, pos.z * CHUNK_DEPTH) + D3DXVECTOR3(CHUNK_WIDTH / 2, CHUNK_HEIGHT / 2, CHUNK_DEPTH / 2);
		float dist = MathHelper::Distance(center, player->pos);
		dist_to_player = dist;

		if (dist < 16.0f)
		{
			Ray ray = Ray(player->cam.pos, D3DXVECTOR3(cosf(player->cam.lookdir.x), player->cam.lookdir.y, sinf(player->cam.lookdir.x)));
			D3DXVec3Normalize(&ray.direction, &ray.direction);

			float reldist = 99999.0f;
			int hit_index = -1;
			D3DXVECTOR3 hitpos = D3DXVECTOR3(-99999, -99999, -99999);

			for (int i = 0; i < CHUNK_SIZE; i++)
			{
				if (blocks[i].id == 0) { continue; }

				int z = BLOCK_Z(i), y = BLOCK_Y(i, z), x = BLOCK_X(i, z, y);
				D3DXVECTOR3 blkwpos = realpos + D3DXVECTOR3(x, y, z);
				bool hit = ray.Intersects(blkwpos);
				float blkdist = MathHelper::Distance(player->pos, blkwpos);

				if (blkdist >= 10.0f) { continue; }
				if (hit && blkdist < reldist) { hit_index = i; hitpos = blkwpos; reldist = blkdist; }
			}

			if (hit_index >= 0 && hit_index < CHUNK_SIZE)
			{
				player->hitpos = hitpos;
				player->hitchunk = this;
				rayhit = true;
			}
			else { rayhit = false; }
		}
		else { rayhit = false; }
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

		size_t rf = 0, tf = 0;
		for (DWORD i = 0; i < mesh.faces.size(); i++)
		{
			ChunkMeshFace& face = mesh.faces[i];
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