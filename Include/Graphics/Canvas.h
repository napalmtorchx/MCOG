#pragma once
#include "Common.h"
#include "Graphics/Vertex.h"
#include "Graphics/Rectangle.h"
#include "Graphics/Texture.h"
#include "Graphics/SpriteFont.h"

namespace Minecraft
{
    class Canvas
    {
        private:
            static LPDIRECT3DVERTEXBUFFER8 _vb;

        public:
            static void Init();
            static void Begin();
            static void End();

        public:
            static void DrawFilledRect(int x, int y, int w, int h, D3DXCOLOR color);
            static void DrawFilledRect(const Rectangle& bnds, D3DXCOLOR color);
            static void Draw(int x, int y, int w, int h, Texture* tex, D3DXCOLOR color);
            static void Draw(const Rectangle& bnds, Texture* tex, D3DXCOLOR color);
            static void Draw(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, Texture* tex, D3DXCOLOR color);
            static void Draw(const Rectangle& dest, const Rectangle& src, Texture* tex, D3DXCOLOR color);
            static void DrawChar(int x, int y, float scale, char c, D3DXCOLOR fg, SpriteFont* font);
            static void DrawChar(const D3DXVECTOR2& pos, float scale, char c, D3DXCOLOR fg, SpriteFont* font);
            static void DrawChar(int x, int y, float scale, char c, D3DXCOLOR fg, D3DXCOLOR bg, SpriteFont* font);
            static void DrawChar(const D3DXVECTOR2& pos, float scale, char c, D3DXCOLOR fg, D3DXCOLOR bg, SpriteFont* font);
            static void DrawString(int x, int y, float scale, LPCSTR str, D3DXCOLOR fg, SpriteFont* font);
            static void DrawString(const D3DXVECTOR2& pos, float scale, LPCSTR str, D3DXCOLOR fg, SpriteFont* font);
            static void DrawStringShadow(int x, int y, float scale, LPCSTR str, D3DXCOLOR fg, D3DXCOLOR shcol, int shdist, SpriteFont* font);
            static void DrawStringShadow(const D3DXVECTOR2& pos, float scale, LPCSTR str, D3DXCOLOR fg, D3DXCOLOR shcol, int shdist, SpriteFont* font);
        };

        static const Vertex QUAD_VERTICES[] =
        {
            { D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0xFFFFFFFF, D3DXVECTOR2(0.0f, 0.0f) },
            { D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0xFFFFFFFF, D3DXVECTOR2(1.0f, 0.0f) },
            { D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xFFFFFFFF, D3DXVECTOR2(0.0f, 1.0f) },
            { D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0xFFFFFFFF, D3DXVECTOR2(1.0f, 0.0f) },
            { D3DXVECTOR3(1.0f, 1.0f, 0.0f), 0xFFFFFFFF, D3DXVECTOR2(1.0f, 1.0f) },
            { D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xFFFFFFFF, D3DXVECTOR2(0.0f, 1.0f) },
        };
}