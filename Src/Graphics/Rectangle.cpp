#include "Graphics/Rectangle.h"
#include "Minecraft.h"

namespace Minecraft
{
    Rectangle::Rectangle() : x(0), y(0), w(0), h(0) {}

    Rectangle::Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

    Rectangle::Rectangle(const Rectangle& r) : x(r.x), y(r.y), w(r.w), h(r.h) {}

    bool Rectangle::Equals(const Rectangle& r1, const Rectangle& r2)
    {
        return r1.x == r2.x && r1.y == r2.y && r1.w == r2.w && r1.h == r2.h;
    }

    bool Rectangle::Equals(const Rectangle& r, int x, int y, int w, int h)
    {
        return r.x == x && r.y == y && r.w == w && r.h == h;
    }

    bool Rectangle::Equals(const Rectangle& r)
    {
        return r.x == x && r.y == y && r.w == w && r.h == h;
    }

    bool Rectangle::Equals(int x, int y, int w, int h) { return Equals(*this, x, y, w, h); }

    bool Rectangle::Contains(const Rectangle& r, int x, int y)
    {
        return (x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h);
    }

    bool Rectangle::Contains(const Rectangle& r, const D3DXVECTOR2& pos)
    {
        return (pos.x >= r.x && pos.x < r.x + r.w && pos.y >= r.y && pos.y < r.y + r.h);
    }

    bool Rectangle::Contains(int x, int y) { return Contains(*this, x, y); }

    bool Rectangle::Contains(const D3DXVECTOR2& pos) { return Contains(*this, pos.x, pos.y); }

}