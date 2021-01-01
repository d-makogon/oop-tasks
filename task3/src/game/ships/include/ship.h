#pragma once

#include <vector>
#include <map>
#include <string>

#include <coordinate.h>
#include <shipTypes.h>
#include <shotStatus.h>

namespace bs
{
    class Ship
    {
    private:
        ShipType shipType;
        std::vector<Coordinate> cells;
        int cellsRemaining;
        int shipSize;

    public:
        Ship(const ShipType& shipType, int shipSize);

        bool IsSunk() const;

        ShotResult Fire(const Coordinate& coordinate);

        int GetSize() const;

        bool IsOnCoordinate(const Coordinate& coord) const;

        void SetCoords(const std::vector<Coordinate>& coords);

        const std::vector<Coordinate>& GetCoords() const;
    };
}
