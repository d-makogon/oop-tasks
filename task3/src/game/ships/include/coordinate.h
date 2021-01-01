#pragma once

namespace bs
{
    class Coordinate
    {
    private:
        int x;
        int y;

    public:
        Coordinate() : x(0), y(0) {}

        Coordinate(int x, int y) : x(x), y(y) {}

        void SetX(int newX);

        void SetY(int newY);

        int GetX() const;

        int GetY() const;

        Coordinate& operator=(const Coordinate& other) = default;

        bool operator==(const Coordinate& other) const;

        bool operator!=(const Coordinate& other) const;

        bool operator<(const Coordinate& other) const;
    };
}