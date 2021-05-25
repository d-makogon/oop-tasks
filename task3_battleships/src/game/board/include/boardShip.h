#pragma once

#include <map>

#include <ship.h>

namespace bs
{
    enum class ShipDirection
    {
        Up, Down, Left, Right
    };

    const std::map<ShipDirection, const char* const> ShipDirsNames = {
            {ShipDirection::Up,    "Up"},
            {ShipDirection::Down,  "Down"},
            {ShipDirection::Left,  "Left"},
            {ShipDirection::Right, "Right"},
    };

    class BoardShip
    {
    private:
        Coordinate coordinate;
        ShipDirection direction;
        ShipType type;

    public:
        BoardShip(const Coordinate& coord, const ShipDirection& dir, const ShipType& shipType) : coordinate(coord),
                                                                                                 direction(dir),
                                                                                                 type(shipType) {}

        Coordinate GetCoordinate() const;

        ShipDirection GetDirection() const;

        ShipType GetType() const;
    };
}

