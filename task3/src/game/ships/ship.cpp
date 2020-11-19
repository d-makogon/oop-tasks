#include <algorithm>
#include "ship.h"

bs::Ship::Ship(const ShipType& shipType, int shipSize)
{
    this->shipType = shipType;
    this->cells.reserve(shipSize);
    this->shipSize = shipSize;
    this->cellsRemaining = shipSize;
}

bool bs::Ship::IsSunk() const
{
    return cellsRemaining == 0;
}

bs::ShotResult bs::Ship::Fire(const Coordinate& coordinate)
{
    for (auto& cell: cells)
    {
        if (cell == coordinate)
        {
            cellsRemaining = (cellsRemaining > 0) ? (cellsRemaining - 1) : 0;

            if (cellsRemaining == 0)
            {
                return ShotResult::HitAndSunk;
            }

            return ShotResult::Hit;
        }
    }
    return ShotResult::Miss;
}

int bs::Ship::GetSize() const
{
    return shipSize;
}

bool bs::Ship::IsOnCoordinate(const bs::Coordinate& coord) const
{
    return std::any_of(cells.begin(), cells.end(), [coord](const Coordinate& cell) { return cell == coord; });
}

void bs::Ship::SetCoords(const std::vector<Coordinate>& coords)
{
    if (coords.size() != GetSize())
    {
        throw std::invalid_argument("Wrong amount of ship cells.");
    }

    cells = coords;
}
