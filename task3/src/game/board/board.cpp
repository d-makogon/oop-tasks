#include "include/board.h"

#include <exception>
#include <algorithm>

#include <shipCreator.h>

bs::Board::Board()
{
    for (auto&[type, maxCount] : ShipsMaxAmount)
    {
        shipsCount[type] = 0;
        maxShips += maxCount;
    }
    ships.reserve(maxShips);
}

bs::ShotResult bs::Board::Fire(const bs::Coordinate& coord)
{
    if (!IsValidCoordinate(coord))
    {
        return ShotResult::Invalid;
    }

    // check if already shot at these coords
    auto it = shotHistory.find(coord);
    if (it != shotHistory.end())
    {
        return ShotResult::Duplicate;
    }

    return CheckShipsForHit(coord);
}

bool bs::Board::IsValidCoordinate(const bs::Coordinate& coord) const
{
    int x = coord.GetX();
    int y = coord.GetY();
    return (x >= 0 && x < xSize) && (y >= 0 && y < ySize);
}

bs::ShotResult bs::Board::CheckShipsForHit(const bs::Coordinate& coord)
{
    ShotResult hitStatus = ShotResult::Miss;

    for (auto& ship : ships)
    {
        if (ship.IsSunk())
        {
            continue;
        }

        hitStatus = ship.Fire(coord);

        if (hitStatus == ShotResult::Hit)
        {
            shotHistory[coord] = ShotHistory::Hit;
        }
        else if (hitStatus == ShotResult::HitAndSunk)
        {
            for (const auto& shipCoord : ship.GetCoords())
            {
                shotHistory[shipCoord] = ShotHistory::HitAndSunk;
            }
        }

        if (hitStatus != ShotResult::Miss)
        {
            break;
        }
    }

    // check if all ships are sunk
    bool allSunk = std::all_of(ships.begin(), ships.end(), [](const Ship& ship) { return ship.IsSunk(); });

    if (allSunk)
    {
        return ShotResult::Victory;
    }

    if (hitStatus == ShotResult::Miss)
    {
        shotHistory[coord] = ShotHistory::Miss;
    }

    return hitStatus;
}

bs::ShipPlacementResult bs::Board::PlaceShip(const bs::BoardShip& boardShip)
{
    if (ships.size() >= maxShips)
    {
        return ShipPlacementResult::MaxAmountReached;
    }

    ShipType type = boardShip.GetType();

    if (shipsCount[type] >= ShipsMaxAmount.at(type))
    {
        return ShipPlacementResult::MaxTypeAmountReached;
    }

    if (!IsValidCoordinate(boardShip.GetCoordinate()))
    {
        return ShipPlacementResult::NotEnoughSpace;
    }

    Ship newShip = ShipCreator::CreateShip(type);

    std::vector<Coordinate> shipCoords = GetShipCoords(boardShip, newShip.GetSize());

    for (auto& coord : shipCoords)
    {
        if (!IsValidCoordinate(coord))
        {
            return ShipPlacementResult::NotEnoughSpace;
        }
        if (!AreSurroundingCellsEmpty(coord))
        {
            return ShipPlacementResult::Overlap;
        }
    }

    newShip.SetCoords(shipCoords);

    ships.push_back(newShip);

    shipsCount[type]++;

    return ShipPlacementResult::Success;
}

bool bs::Board::CheckForShip(const Coordinate& coord) const
{
    return std::any_of(ships.begin(), ships.end(), [coord](const Ship& ship) { return ship.IsOnCoordinate(coord); });
}

std::vector<bs::Coordinate> bs::Board::GetShipCoords(const bs::BoardShip& ship, int shipSize)
{
    std::vector<Coordinate> coords;
    coords.reserve(shipSize);
    auto dir = ship.GetDirection();
    auto start = ship.GetCoordinate();
    auto startX = start.GetX();
    auto startY = start.GetY();

    for (size_t i = 0; i < shipSize; i++)
    {
        switch (dir)
        {
            case ShipDirection::Right:
                coords.emplace_back(startX + i, startY);
                break;
            case ShipDirection::Left:
                coords.emplace_back(startX - i, startY);
                break;
            case ShipDirection::Down:
                coords.emplace_back(startX, startY + i);
                break;
            case ShipDirection::Up:
                coords.emplace_back(startX, startY - i);
                break;
            default:
                throw std::invalid_argument("Unknown ship direction.");
        }
    }

    return coords;
}

std::vector<bs::Coordinate> bs::Board::GetSurroundingCells(const bs::Coordinate& coord) const
{
    if (!IsValidCoordinate(coord)) throw std::invalid_argument("Coordinate out of board");

    int x = coord.GetX();
    int y = coord.GetY();

    // @formatter:off
    std::vector<Coordinate> surCells = {
            {x - 1, y - 1}, {x, y - 1}, {x + 1, y - 1},
            {x - 1, y},     {x, y},     {x + 1, y},
            {x - 1, y + 1}, {x, y + 1}, {x + 1, y + 1}
    };
    // @formatter:on

    for (auto it = surCells.begin(); it != surCells.end();)
    {
        if (!IsValidCoordinate(*it))
            surCells.erase(it);
        else
            it++;
    }

    return surCells;
}

bool bs::Board::AreSurroundingCellsEmpty(const Coordinate& coord) const
{
    auto surCells = GetSurroundingCells(coord);

    // return true if none of surrounding cells has a ship on it
    return std::none_of(surCells.begin(), surCells.end(), [this](const Coordinate& c) { return CheckForShip(c); });
}

bs::ShotHistory bs::Board::GetHistoryAt(const Coordinate& coord) const
{
    auto it = shotHistory.find(coord);

    if (it != shotHistory.end())
    {
        return it->second;
    }
    return ShotHistory::Unknown;
}

bool bs::Board::CanPlaceShips() const
{
    return ships.size() < maxShips;
}

std::vector<std::pair<bs::ShipType, int>> bs::Board::GetAvailableShipsAmount() const
{
    std::vector<std::pair<bs::ShipType, int>> result;
    for (auto&[type, count] : shipsCount)
    {
        result.emplace_back(type, ShipsMaxAmount.at(type) - count);
    }
    return result;
}

bool bs::Board::CanPlaceShipOfType(const bs::ShipType& type) const
{
    return shipsCount.at(type) < ShipsMaxAmount.at(type);
}

const std::map<bs::Coordinate, bs::ShotHistory>& bs::Board::GetShotHistory() const
{
    return shotHistory;
}
