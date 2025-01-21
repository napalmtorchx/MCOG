#include "Graphics/Canvas.h"
#include "Minecraft.h"

namespace Minecraft
{
    LPDIRECT3DVERTEXBUFFER8 Canvas::_vb;

    void Canvas::Init()
    {
        if (FAILED(GraphicsManager::device->CreateVertexBuffer(6 * sizeof(Vertex), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &_vb)))
        {
            RXDK_LOG_ERROR("Failed to create vertex buffer for 2D canvas");
            return;
        }

        RXDK_LOG_INFO("Initialized 2D canvas\n");
    }

    void Canvas::Begin()
    {
        // Begin context
        GraphicsManager::device->BeginScene();

        // Configure texture stage properties
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        GraphicsManager::device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

        // Configure render state
        GraphicsManager::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        GraphicsManager::device->SetRenderState(D3DRS_LIGHTING, FALSE);
        GraphicsManager::device->SetRenderState(D3DRS_ZENABLE, FALSE);
        GraphicsManager::device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        GraphicsManager::device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        GraphicsManager::device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // Create 2D orthographic projection
        D3DXMATRIX m_view, m_ident;
        D3DXMatrixIdentity(&m_ident);
        D3DXMatrixOrthoOffCenterLH(&m_view, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0f, 1.0f);

        // Apply projection to graphics device
        GraphicsManager::device->SetTransform(D3DTS_PROJECTION, &m_view);
        GraphicsManager::device->SetTransform(D3DTS_VIEW, &m_ident);
        GraphicsManager::device->SetTransform(D3DTS_WORLD, &m_ident);
    }

    void Canvas::End()
    {
        // End context
        GraphicsManager::device->EndScene();
    }

    void Canvas::DrawFilledRect(int x, int y, int w, int h, D3DXCOLOR color)
    {
        Vertex* vertices = NULL;
        if (FAILED(_vb->Lock(0, 0, (BYTE**)&vertices, 0))) { return; }
        for (int i = 0; i < 6; i++)
        {
            vertices[i].pos.x = x + QUAD_VERTICES[i].pos.x * w;
            vertices[i].pos.y = y + QUAD_VERTICES[i].pos.y * h;
            vertices[i].pos.z = QUAD_VERTICES[i].pos.z;
            vertices[i].uv = D3DXVECTOR2(QUAD_VERTICES[i].uv.x, QUAD_VERTICES[i].uv.y);
            vertices[i].color = color;
        }
        _vb->Unlock();

        GraphicsManager::device->SetTexture(0, NULL);
        GraphicsManager::device->SetStreamSource(0, _vb, sizeof(Vertex));
        GraphicsManager::device->SetVertexShader(D3DFVF_CUSTOMVERTEX);
        GraphicsManager::device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    }

    void Canvas::DrawFilledRect(const Rectangle& bnds, D3DXCOLOR color) { DrawFilledRect(bnds.x, bnds.y, bnds.w, bnds.h, color); }

    void Canvas::Draw(int x, int y, int w, int h, Texture* tex, D3DXCOLOR color)
    {
        Vertex* vertices = NULL;
        if (FAILED(_vb->Lock(0, 0, (BYTE**)&vertices, 0))) { return; }
        for (int i = 0; i < 6; i++)
        {
            vertices[i].pos.x = x + QUAD_VERTICES[i].pos.x * w;
            vertices[i].pos.y = y + QUAD_VERTICES[i].pos.y * h;
            vertices[i].pos.z = QUAD_VERTICES[i].pos.z;
            vertices[i].uv = D3DXVECTOR2(QUAD_VERTICES[i].uv.x, QUAD_VERTICES[i].uv.y);
            vertices[i].color = color;
        }
        _vb->Unlock();

        GraphicsManager::device->SetTexture(0, tex->D3D());
        GraphicsManager::device->SetStreamSource(0, _vb, sizeof(Vertex));
        GraphicsManager::device->SetVertexShader(D3DFVF_CUSTOMVERTEX);
        GraphicsManager::device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    }

    void Canvas::Draw(const Rectangle& bnds, Texture* tex, D3DXCOLOR color) { Draw(bnds.x, bnds.y, bnds.w, bnds.h, tex, color); }

    void Canvas::Draw(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, Texture* tex, D3DXCOLOR color)
    {
        float u = (float)sx / (int)tex->Size().x;
        float v = (float)sy / (int)tex->Size().y;
        float um = (float)(sx + sw) / (int)tex->Size().x;
        float vm = (float)(sy + sh) / (int)tex->Size().y;

        D3DXVECTOR2 uvs[6] =
        {
            D3DXVECTOR2(u,  v),
            D3DXVECTOR2(um, v),
            D3DXVECTOR2(u,  vm),
            D3DXVECTOR2(um, v),
            D3DXVECTOR2(um, vm),
            D3DXVECTOR2(u,  vm),
        };

        Vertex* vertices = NULL;
        if (FAILED(_vb->Lock(0, 0, (BYTE**)&vertices, 0))) { return; }
        for (int i = 0; i < 6; i++)
        {
            vertices[i].pos.x = dx + QUAD_VERTICES[i].pos.x * dw;
            vertices[i].pos.y = dy + QUAD_VERTICES[i].pos.y * dh;
            vertices[i].pos.z = QUAD_VERTICES[i].pos.z;
            vertices[i].uv = uvs[i];
            vertices[i].color = color;
        }
        _vb->Unlock();

        GraphicsManager::device->SetTexture(0, tex->D3D());
        GraphicsManager::device->SetStreamSource(0, _vb, sizeof(Vertex));
        GraphicsManager::device->SetVertexShader(D3DFVF_CUSTOMVERTEX);
        GraphicsManager::device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    }

    void Canvas::Draw(const Rectangle& dest, const Rectangle& src, Texture* tex, D3DXCOLOR color) { Draw(dest.x, dest.y, dest.w, dest.h, src.x, src.y, src.w, src.h, tex, color); }

    void Canvas::DrawChar(int x, int y, float scale, char c, D3DXCOLOR fg, SpriteFont* font)
    {
        float w = scale * (int)font->sz.x, h = scale * (int)font->sz.y;

        Rectangle bnds = Rectangle(x, y, (int)w, (int)h);
        Rectangle glyph = font->Glyph(c);
        Draw(bnds, glyph, font->texture, fg);
    }

    void Canvas::DrawChar(const D3DXVECTOR2& pos, float scale, char c, D3DXCOLOR fg, SpriteFont* font) { DrawChar(pos.x, pos.y, scale, c, fg, font); }

    void Canvas::DrawChar(int x, int y, float scale, char c, D3DXCOLOR fg, D3DXCOLOR bg, SpriteFont* font)
    {
        float w = scale * (int)font->sz.x, h = scale * (int)font->sz.y;

        Rectangle bnds = Rectangle(x, y, (int)w, (int)h);
        Rectangle glyph = font->Glyph(c);
        DrawFilledRect(bnds, bg);
        Draw(bnds, glyph, font->texture, fg);
    }

    void Canvas::DrawChar(const D3DXVECTOR2& pos, float scale, char c, D3DXCOLOR fg, D3DXCOLOR bg, SpriteFont* font) { DrawChar(pos.x, pos.y, scale, c, fg, bg, font); }

    void Canvas::DrawString(int x, int y, float scale, LPCSTR str, D3DXCOLOR fg, SpriteFont* font)
    {
        int i = 0, xx = x, yy = y;
        while (str[i] != 0)
        {
            if (str[i] == '\n') { xx = x; yy += (int)(scale * ((int)font->sz.y + (int)font->spacing.y)); }
            else
            {
                DrawChar(xx, yy, scale, str[i], fg, font);
                xx += (int)(scale * ((int)font->sz.x + (int)font->spacing.x));
            }
            i++;
        }
    }

    void Canvas::DrawString(const D3DXVECTOR2& pos, float scale, LPCSTR str, D3DXCOLOR fg, SpriteFont* font) { DrawString(pos.x, pos.y, scale, str, fg, font); }

    void Canvas::DrawStringShadow(int x, int y, float scale, LPCSTR str, D3DXCOLOR fg, D3DXCOLOR shcol, int shdist, SpriteFont* font)
    {
        DrawString(x + shdist, y + shdist, scale, str, shcol, font);
        DrawString(x, y, scale, str, fg, font);
    }

    void Canvas::DrawStringShadow(const D3DXVECTOR2& pos, float scale, LPCSTR str, D3DXCOLOR fg, D3DXCOLOR shcol, int shdist, SpriteFont* font)
    {
        DrawStringShadow(pos.x, pos.y, scale, str, fg, shcol, shdist, font);
    }
}