#include "World/ChunkMesh.h"
#include "Minecraft.h"

namespace Minecraft
{
    ChunkMeshFaceKey::ChunkMeshFaceKey() : face(0), index(-1) { }
    
    ChunkMeshFaceKey::ChunkMeshFaceKey(int index, BYTE face) : face(face), index(index) { }

    bool ChunkMeshFaceKey::operator <(const ChunkMeshFaceKey& other) const
    {
        DWORD i = (DWORD)(index << 5) | (DWORD)face;
        DWORD j = (DWORD)(other.index << 5) | (DWORD)other.face;
        return i < j;
    }

    // ------------------------------------------------------------------------------------------------------------------------------

    ChunkMeshFace::ChunkMeshFace()
    {
        face    = 0;
        id      = BLOCKID_AIR;
        sz      = D3DXVECTOR3(0, 0, 0);
        texture = NULL;
        memset(this->vertices, 0, sizeof(this->vertices));
    }

    ChunkMeshFace::ChunkMeshFace(BYTE face, BYTE type, Texture* texture)
    {
        this->face    = face;
        this->id      = type;
        this->sz      = D3DXVECTOR3(1, 1, 1);
        this->texture = texture;
        memset(this->vertices, 0, sizeof(this->vertices));
    }

    // ------------------------------------------------------------------------------------------------------------------------------

    ChunkMesh::ChunkMesh() : chunk(NULL), generated(false)
    {
        memset(_tempfaces, 0, sizeof(_tempfaces));
        memset(_sidefaces, 0, sizeof(_sidefaces));
        facemap.clear();
        faces.clear();
    }

    ChunkMesh::ChunkMesh(Chunk* chunk) : chunk(chunk), generated(false)
    {
        memset(_tempfaces, 0, sizeof(_tempfaces));
        memset(_sidefaces, 0, sizeof(_sidefaces));
        facemap.clear();
        faces.clear();
    }

    void ChunkMesh::Generate()
    {
        memset(_tempfaces, 0, sizeof(_tempfaces));
        memset(_sidefaces, 0, sizeof(_sidefaces));
        facemap.clear();
        faces.clear();
        generated = false;
        DWORD bt = 0;
        int i, j, x, y, z;

        for (i = 0; i < CHUNK_SIZE; i++)
        {
            if (chunk->blocks[i].id == BLOCKID_AIR) { continue; }
            bt++;
        }

        if (bt > 0)
        {
            for (i = 0; i < CHUNK_SIZE; i++)
            {
                if (chunk->blocks[i].id == BLOCKID_AIR) { continue; }

                z = BLOCK_Z(i);
                y = BLOCK_Y(i, z);
                x = BLOCK_X(i, z, y);

                /*
                Block* blks[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
                int n = GetNeighbors(blks, x, y, z);
                if (n == 6 && 
                    x > 0 && x < CHUNK_WIDTH - 1 && 
                    y > 0 && y < CHUNK_HEIGHT - 1 &&
                    z > 0 && z < CHUNK_DEPTH - 1) { continue; }
                    */

                BlockMetadata& metadata = Assets::blocks[chunk->blocks[i].id];
                GenerateBlockFaces(&metadata, &chunk->blocks[i], x, y, z);

                for (j = 0; j < VOXEL_FACE_COUNT; j++)
                {
                    if (_tempfaces[j].id == BLOCKID_AIR) { continue; }
                    _tempfaces[j].sz = D3DXVECTOR3(1, 1, 1);

                    ChunkMeshFaceKey key(i, _tempfaces[j].face);
                    facemap[key] = _tempfaces[j];
                }
            }

            GreedyMesh();

            D3DXCOLOR color;
            float tw, th;
            int v;

            std::map<ChunkMeshFaceKey, ChunkMeshFace>::iterator it;
            for (it = facemap.begin(); it != facemap.end(); ++it)
            {
                ChunkMeshFace& face = it->second;
                if (face.sz.y == -1 || face.sz.z == -1 || face.sz.x == -1) { continue; }

                tw = 1;
                th = 1;
                float cm = 1.0f;
                if (face.face == VOXELFACE_FRONT) { cm = 0.9f; }
                if (face.face == VOXELFACE_LEFT || face.face == VOXELFACE_RIGHT) { cm = 0.6f; }
                if (face.face == VOXELFACE_BACK) { cm = 0.4f; }
                if (face.face == VOXELFACE_BOTTOM) { cm = 0.32f; }

                if (face.face == VOXELFACE_FRONT || face.face == VOXELFACE_BACK) { tw = face.sz.x; th = face.sz.y; }
                else if (face.face == VOXELFACE_LEFT || face.face == VOXELFACE_RIGHT) { tw = face.sz.z; th = face.sz.y; }
                else if (face.face == VOXELFACE_BOTTOM) { tw = face.sz.z; th = face.sz.x; }
                else if (face.face == VOXELFACE_TOP) { tw = face.sz.z; th = face.sz.x; }

                for (v = 0; v < FACE_VERTEX_COUNT; v++)
                {
                    face.vertices[v].pos.x += VOXEL_VERTICES[(face.face * FACE_VERTEX_COUNT) + v].x * face.sz.x;
                    face.vertices[v].pos.y += VOXEL_VERTICES[(face.face * FACE_VERTEX_COUNT) + v].y * face.sz.y;
                    face.vertices[v].pos.z += VOXEL_VERTICES[(face.face * FACE_VERTEX_COUNT) + v].z * face.sz.z;
                    face.vertices[v].uv.x = VOXEL_UVS[(face.face * FACE_VERTEX_COUNT) + v].x * tw;
                    face.vertices[v].uv.y = VOXEL_UVS[(face.face * FACE_VERTEX_COUNT) + v].y * th;

                    color.r = cm;
                    color.g = cm;
                    color.b = cm;
                    color.a = 0xFF;

                    face.vertices[v].color = color;
                }

                faces.push_back(face);                
            }
        }

        facemap.clear();
        generated = true;
    }

    void ChunkMesh::GenerateBlockFaces(BlockMetadata* metadata, Block* blk, int x, int y, int z)
    {
        memset(_tempfaces, 0, sizeof(_tempfaces));

        D3DXVECTOR3 chunkpos = D3DXVECTOR3(chunk->pos.x * CHUNK_WIDTH, chunk->pos.y * CHUNK_HEIGHT, chunk->pos.z * CHUNK_DEPTH);
        D3DXVECTOR3 pos = D3DXVECTOR3(chunkpos.x + x, chunkpos.y + y, chunkpos.z + z);

        _tempfaces[VOXELFACE_FRONT]  = ChunkMeshFace(VOXELFACE_FRONT, blk->id, metadata->textures[VOXELFACE_FRONT]);
        _tempfaces[VOXELFACE_RIGHT]  = ChunkMeshFace(VOXELFACE_RIGHT, blk->id, metadata->textures[VOXELFACE_RIGHT]);
        _tempfaces[VOXELFACE_BACK]   = ChunkMeshFace(VOXELFACE_BACK, blk->id, metadata->textures[VOXELFACE_BACK]);
        _tempfaces[VOXELFACE_LEFT]   = ChunkMeshFace(VOXELFACE_LEFT, blk->id, metadata->textures[VOXELFACE_LEFT]);
        _tempfaces[VOXELFACE_TOP]    = ChunkMeshFace(VOXELFACE_TOP, blk->id, metadata->textures[VOXELFACE_TOP]);
        _tempfaces[VOXELFACE_BOTTOM] = ChunkMeshFace(VOXELFACE_BOTTOM, blk->id, metadata->textures[VOXELFACE_BOTTOM]);

        int i, j;
        for (i = 0; i < VOXEL_FACE_COUNT; i++)
        {
            for (j = 0; j < FACE_VERTEX_COUNT; j++) { _tempfaces[i].vertices[j].pos = pos; }
        }
    }

    void ChunkMesh::GreedyMesh()
    {
        int x, y, z;


        memset(_sidefaces, 0, sizeof(_sidefaces));

        for (z = 0; z < CHUNK_DEPTH; z++)
        {
            for (x = 0; x < CHUNK_WIDTH; x++)
            {
                for (y = CHUNK_HEIGHT - 2; y >= 0; y--) { GreedyMeshY(x, y, z); }
            }
        }

        for (z = 0; z < CHUNK_DEPTH; z++)
        {
            for (y = 0; y < CHUNK_HEIGHT; y++)
            {
                for (x = CHUNK_WIDTH - 2; x >= 0; x--) { GreedyMeshX(x, y, z); }
            }
        }

        for (y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (x = 0; x < CHUNK_WIDTH; x++)
            {
                for (z = CHUNK_DEPTH - 2; z >= 0; z--) { GreedyMeshZ(x, y, z); }
            }
        }
    }

    void ChunkMesh::GreedyMeshX(int x, int y, int z)
    {
        _sidefaces[0] = GetFace(x, y, z, VOXELFACE_FRONT);
        _sidefaces[1] = GetFace(x + 1, y, z, VOXELFACE_FRONT);
        _sidefaces[2] = GetFace(x, y, z, VOXELFACE_RIGHT);
        _sidefaces[3] = GetFace(x + 1, y, z, VOXELFACE_RIGHT);
        _sidefaces[4] = GetFace(x, y, z, VOXELFACE_BACK);
        _sidefaces[5] = GetFace(x + 1, y, z, VOXELFACE_BACK);
        _sidefaces[6] = GetFace(x, y, z, VOXELFACE_LEFT);
        _sidefaces[7] = GetFace(x + 1, y, z, VOXELFACE_LEFT);
        _sidefaces[8] = GetFace(x, y, z, VOXELFACE_TOP);
        _sidefaces[9] = GetFace(x + 1, y, z, VOXELFACE_TOP);
        _sidefaces[10] = GetFace(x, y, z, VOXELFACE_BOTTOM);
        _sidefaces[11] = GetFace(x + 1, y, z, VOXELFACE_BOTTOM);

        for (int i = 0; i < 12; i += 2)
        {
            if (_sidefaces[i] == NULL || _sidefaces[i + 1] == NULL) { continue; }
            if (_sidefaces[i]->id != _sidefaces[i + 1]->id) { continue; }
            if (_sidefaces[i]->sz.x == -1 || _sidefaces[i]->sz.y == -1) { continue; }
            if (_sidefaces[i]->sz.y != _sidefaces[i + 1]->sz.y) { continue; }

            _sidefaces[i]->sz.x += _sidefaces[i + 1]->sz.x;
            _sidefaces[i + 1]->sz.x = -1;

        }
    }

    void ChunkMesh::GreedyMeshY(int x, int y, int z)
    {
        _sidefaces[0] = GetFace(x, y, z, VOXELFACE_FRONT);
        _sidefaces[1] = GetFace(x, y + 1, z, VOXELFACE_FRONT);
        _sidefaces[2] = GetFace(x, y, z, VOXELFACE_RIGHT);
        _sidefaces[3] = GetFace(x, y + 1, z, VOXELFACE_RIGHT);
        _sidefaces[4] = GetFace(x, y, z, VOXELFACE_BACK);
        _sidefaces[5] = GetFace(x, y + 1, z, VOXELFACE_BACK);
        _sidefaces[6] = GetFace(x, y, z, VOXELFACE_LEFT);
        _sidefaces[7] = GetFace(x, y + 1, z, VOXELFACE_LEFT);
        _sidefaces[8] = GetFace(x, y, z, VOXELFACE_TOP);
        _sidefaces[9] = GetFace(x, y + 1, z, VOXELFACE_TOP);
        _sidefaces[10] = GetFace(x, y, z, VOXELFACE_BOTTOM);
        _sidefaces[11] = GetFace(x, y + 1, z, VOXELFACE_BOTTOM);

        for (int i = 0; i < 12; i += 2)
        {
            if (_sidefaces[i] == NULL || _sidefaces[i + 1] == NULL) { continue; }
            if (_sidefaces[i]->id != _sidefaces[i + 1]->id) { continue; }
            if (_sidefaces[i]->sz.y == -1) { continue; }

            _sidefaces[i]->sz.y += _sidefaces[i + 1]->sz.y;
            _sidefaces[i + 1]->sz.y = -1;
        }
    }

    void ChunkMesh::GreedyMeshZ(int x, int y, int z)
    {
        _sidefaces[0] = GetFace(x, y, z, VOXELFACE_FRONT);
        _sidefaces[1] = GetFace(x, y, z + 1, VOXELFACE_FRONT);
        _sidefaces[2] = GetFace(x, y, z, VOXELFACE_RIGHT);
        _sidefaces[3] = GetFace(x, y, z + 1, VOXELFACE_RIGHT);
        _sidefaces[4] = GetFace(x, y, z, VOXELFACE_BACK);
        _sidefaces[5] = GetFace(x, y, z + 1, VOXELFACE_BACK);
        _sidefaces[6] = GetFace(x, y, z, VOXELFACE_LEFT);
        _sidefaces[7] = GetFace(x, y, z + 1, VOXELFACE_LEFT);
        _sidefaces[8] = GetFace(x, y, z, VOXELFACE_TOP);
        _sidefaces[9] = GetFace(x, y, z + 1, VOXELFACE_TOP);
        _sidefaces[10] = GetFace(x, y, z, VOXELFACE_BOTTOM);
        _sidefaces[11] = GetFace(x, y, z + 1, VOXELFACE_BOTTOM);

        for (int i = 0; i < 12; i += 2)
        {
            if (_sidefaces[i] == NULL || _sidefaces[i + 1] == NULL) { continue; }
            if (_sidefaces[i]->id != _sidefaces[i + 1]->id) { continue; }
            if (_sidefaces[i]->sz.x == -1 || _sidefaces[i]->sz.y == -1 || _sidefaces[i]->sz.z == -1) { continue; }
            if (_sidefaces[i]->sz.y != _sidefaces[i + 1]->sz.y || _sidefaces[i]->sz.x != _sidefaces[i + 1]->sz.x) { continue; }

            _sidefaces[i]->sz.z += _sidefaces[i + 1]->sz.z;
            _sidefaces[i + 1]->sz.z = -1;
        }
    }

    int ChunkMesh::GetNeighbors(Block** blks, int x, int y, int z)
    {
        blks[0] = chunk->Get(x, y, z + 1);
        blks[1] = chunk->Get(x + 1, y, z);
        blks[2] = chunk->Get(x, y, z - 1);
        blks[3] = chunk->Get(x - 1, y, z);
        blks[4] = chunk->Get(x, y + 1, z);
        blks[5] = chunk->Get(x, y - 1, z);

        int c = 0;
        for (int i = 0; i < 6; i++) { if (blks[i] != NULL && blks[i]->id != BLOCKID_AIR) { c++; } }
        return c;
    }

    ChunkMeshFace* ChunkMesh::GetFace(int x, int y, int z, BYTE face)
    {
        ChunkMeshFaceKey key(BLOCK_INDEX(x, y, z), face);
        std::map<ChunkMeshFaceKey, ChunkMeshFace>::iterator it = facemap.find(key);
        if (it != facemap.end()) { return &(it->second); }
        return NULL;
    }
}