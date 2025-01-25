#include "World/World.h"
#include "Minecraft.h"

namespace Minecraft
{
    World::World() : Component(), chunks(NULL), this_chunk(NULL)
    {

    }

    World::World(LPCSTR name) : Component(name), chunks(NULL), this_chunk(NULL)
    {

    }

    void World::Load()
    {
        Component::Load();

        size_t mem = App::GetFreeMemoryAmount();

        chunks = new Chunk[WORLD_SIZE_CHUNKS];
        WorldGenerator::GenerateTerrain(this, 69420);

        size_t mem_after = App::GetFreeMemoryAmount();
        RXDK_LOG_INFO("SIZE OF WORLD: %u bytes\n", mem - mem_after);
    }

    void World::Unload()
    {
        Component::Unload();
        if (chunks != NULL) { delete[] chunks; chunks = NULL; }
    }

    void World::Update(float dt)
    {
        Component::Update(dt);

        int hits = 0;
        Chunk* c = NULL;
        for (int i = 0; i < WORLD_SIZE_CHUNKS; i++)
        {
            if (!chunks[i].loaded) { return; }
            if (chunks[i].contains_player) { c = &chunks[i]; }
            if (chunks[i].rayhit) { hits++; }
            chunks[i].Update(dt);
        }
        this_chunk = c;
        if (hits == 0) { GameState::instance->player.hitchunk = NULL; GameState::instance->player.hitpos = D3DXVECTOR3(-99999, -99999, -99999); }
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

        size_t rf = 0, tf = 0;

        Camera* cam = &GameState::instance->player.cam;
        D3DXMATRIX m_proj, m_mvp;
        D3DXMatrixPerspectiveFovLH(&m_proj, MathHelper::DegreesToRadians(cam->fov / 2), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, cam->clipping[CLIPPING_NEAR], cam->clipping[CLIPPING_FAR]);
        m_mvp = cam->view_matrix * m_proj;
        D3DXMatrixTranspose(&m_mvp, &m_mvp);
        GameState::instance->player.frustum.Extract(&m_mvp);

        int x, y, z;
        for (int i = 0; i < WORLD_SIZE_CHUNKS; i++)
        {
            if (!chunks[i].loaded) { return; }
            z = CHUNK_Z(i, MAX_CHUNKS_RENDERED_X, MAX_CHUNKS_RENDERED_Y);
            y = CHUNK_Y(i, z, MAX_CHUNKS_RENDERED_X, MAX_CHUNKS_RENDERED_Y);
            x = CHUNK_X(i, z, y, MAX_CHUNKS_RENDERED_X, MAX_CHUNKS_RENDERED_Y);

            D3DXVECTOR3 center = MathHelper::Multiply(chunks[i].pos, D3DXVECTOR3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH)) + D3DXVECTOR3(CHUNK_WIDTH / 2, 0, CHUNK_DEPTH / 2);
            float dist = MathHelper::Distance(center, GameState::instance->player.pos);
            if (dist >= 48.0f) { continue; }
            if (!GameState::instance->player.frustum.Contains(center) && dist >= 32.0f) { continue; }

            chunks[i].Draw(dt);
            rf += chunks[i].GetRenderedFaceCount();
            tf += chunks[i].GetTotalFaceCount();
        }

        _rend_faces = rf;
        _total_faces = tf;

    }

    void World::SetBlock(int x, int y, int z, BYTE type)
    {
        int cx = x / CHUNK_WIDTH, cy = y / CHUNK_HEIGHT, cz = z / CHUNK_DEPTH;
        int ci = cx + (int)WORLD_WIDTH_CHUNKS * (cy + (int)WORLD_HEIGHT_CHUNKS * cz);

        if (chunks[ci].blocks == NULL) { return; }

        int bx = x % CHUNK_WIDTH, by = y % CHUNK_HEIGHT, bz = z % CHUNK_DEPTH;
        chunks[ci].blocks[BLOCK_INDEX(bx, by, bz)] = Block(type);
    }

    Block* World::GetBlock(int x, int y, int z, Chunk** chunk)
    {
        if (x < 0 || y < 0 || z < 0) { return NULL; }
        if (x >= CHUNK_WIDTH * WORLD_WIDTH_CHUNKS || y >= CHUNK_HEIGHT * WORLD_HEIGHT_CHUNKS || z >= CHUNK_DEPTH * WORLD_DEPTH_CHUNKS) { return NULL; }

        int cx = x / CHUNK_WIDTH, cy = y / CHUNK_HEIGHT, cz = z / CHUNK_DEPTH;
        int ci = cx + (int)WORLD_WIDTH_CHUNKS * (cy + (int)WORLD_HEIGHT_CHUNKS * cz);

        if (chunks[ci].blocks == NULL) { return NULL; }
        if (chunk != NULL) { *chunk = &chunks[ci]; }

        int bx = x % CHUNK_WIDTH, by = y % CHUNK_HEIGHT, bz = z % CHUNK_DEPTH;
        return &chunks[ci].blocks[BLOCK_INDEX(bx, by, bz)];
    }

    size_t World::GetRenderedFaceCount() { return _rend_faces; }

    size_t World::GetTotalFaceCount() { return _total_faces; }
}