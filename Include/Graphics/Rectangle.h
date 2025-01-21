#pragma once
#include "Common.h"

namespace Minecraft
{
    struct Rectangle
    {
    public:
        int x, y, w, h;
        Rectangle();
        Rectangle(int x, int y, int w, int h);
        Rectangle(const Rectangle& r);

    public:
        static bool Equals(const Rectangle& r1, const Rectangle& r2);
        static bool Equals(const Rectangle& r, int x, int y, int w, int h);
        bool Equals(const Rectangle& r);
        bool Equals(int x, int y, int w, int h);

    public:
        static bool Contains(const Rectangle& r, int x, int y);
        static bool Contains(const Rectangle& r, const D3DXVECTOR2& pos);
        bool Contains(int x, int y);
        bool Contains(const D3DXVECTOR2& pos);
    };
}