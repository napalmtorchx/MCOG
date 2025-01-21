#include "World/World.h"
#include "Minecraft.h"

namespace Minecraft
{
    World::World() : Component(), chunks(NULL)
    {

    }

    World::World(LPCSTR name) : Component(name), chunks(NULL)
    {

    }

    void World::Load()
    {
        Component::Load();

        chunks = new Chunk[WORLD_SIZE_CHUNKS];
        for (size_t i = 0; i < WORLD_SIZE_CHUNKS; i++)
        {
            int z = CHUNK_Z(i, WORLD_WIDTH_CHUNKS, WORLD_HEIGHT_CHUNKS);
            int y = CHUNK_Y(i, z, WORLD_WIDTH_CHUNKS, WORLD_HEIGHT_CHUNKS);
            int x = CHUNK_X(i, z, y, WORLD_WIDTH_CHUNKS, WORLD_HEIGHT_CHUNKS);
            chunks[i] = Chunk(x, y, z);
            chunks[i].Load();
            RXDK_LOG_INFO("Generated chunk %u(%d, %d, %d)\n", i, x, y, z);
        }
    }

    void World::Unload()
    {
        Component::Unload();
        if (chunks != NULL) { delete[] chunks; chunks = NULL; }
    }

    void World::Update(float dt)
    {
        Component::Update(dt);
        for (int i = 0; i < WORLD_SIZE_CHUNKS; i++)
        {
            if (!chunks[i].loaded) { return; }
            chunks[i].Update(dt);
        }
    }

    void World::UpdateFixed()
    {
        Component::UpdateFixed();
        for (int i = 0; i < WORLD_SIZE_CHUNKS; i++)
        {
            if (!chunks[i].loaded) { return; }
            chunks[i].UpdateFixed();
        }
    }

    void World::Draw(float dt)
    {
        Component::Draw(dt);

        Camera* cam = &GameState::instance->player.cam;

        D3DXMATRIX m_proj, m_mvp;
        float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
        float fov = MathHelper::DegreesToRadians(cam->fov);

        D3DXMatrixPerspectiveFovLH(&m_proj, fov, aspect, 0.01f, 1000.0f);
        m_mvp = cam->view_matrix * m_proj;
        D3DXMatrixTranspose(&m_mvp, &m_mvp);

        Frustum frustum;
        frustum.Extract(&m_mvp);

        int x, y, z;
        for (int i = 0; i < WORLD_SIZE_CHUNKS; i++)
        {
            if (!chunks[i].loaded) { return; }
            z = CHUNK_Z(i, MAX_CHUNKS_RENDERED_X, MAX_CHUNKS_RENDERED_Y);
            y = CHUNK_Y(i, z, MAX_CHUNKS_RENDERED_X, MAX_CHUNKS_RENDERED_Y);
            x = CHUNK_X(i, z, y, MAX_CHUNKS_RENDERED_X, MAX_CHUNKS_RENDERED_Y);

            //D3DXVECTOR3 center = MathHelper::Multiply(chunks[i].pos, D3DXVECTOR3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH));
            //center += D3DXVECTOR3(CHUNK_WIDTH / 2, CHUNK_HEIGHT / 2, CHUNK_DEPTH / 2);
            //float dist = MathHelper::Distance(center, GameState::instance->player.pos);
            //if (dist >= 64.0f) { continue; }
            //if (!frustum.Contains(center) && dist > 24.0f) { continue; }

            chunks[i].Draw(dt);
        }
    }

    void World::SetBlock(int x, int y, int z, BYTE type)
    {
        int cx = x / CHUNK_WIDTH, cy = y / CHUNK_HEIGHT, cz = z / CHUNK_DEPTH;
        int ci = cx + (int)MAX_CHUNKS_RENDERED_X * (cy + (int)MAX_CHUNKS_RENDERED_Y * cz);

        if (chunks[ci].blocks == NULL) { return; }

        int bx = x % CHUNK_WIDTH, by = y % CHUNK_HEIGHT, bz = z % CHUNK_DEPTH;
        chunks[ci].blocks[BLOCK_INDEX(bx, by, bz)] = Block(type);
    }

    Block* World::GetBlock(int x, int y, int z, Chunk** chunk)
    {
        if (x < 0 || y < 0 || z < 0) { return NULL; }
        if (x >= CHUNK_WIDTH * MAX_CHUNKS_RENDERED_X || y >= CHUNK_HEIGHT * MAX_CHUNKS_RENDERED_Y || z >= CHUNK_DEPTH * MAX_CHUNKS_RENDERED_Z) { return NULL; }

        int cx = x / CHUNK_WIDTH, cy = y / CHUNK_HEIGHT, cz = z / CHUNK_DEPTH;
        int ci = cx + (int)MAX_CHUNKS_RENDERED_X * (cy + (int)MAX_CHUNKS_RENDERED_Y * cz);

        if (chunks[ci].blocks == NULL) { return NULL; }
        if (chunk != NULL) { *chunk = &chunks[ci]; }

        int bx = x % CHUNK_WIDTH, by = y % CHUNK_HEIGHT, bz = z % CHUNK_DEPTH;
        return &chunks[ci].blocks[BLOCK_INDEX(bx, by, bz)];
    }
}