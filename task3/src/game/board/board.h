#pragma once

#include <vector>
#include <map>

#include "../ships/ship.h"
#include "boardShip.h"

namespace bs
{
    enum class ShotHistory
    {
        Hit, Miss, Unknown
    };

    enum class ShipPlacementResult
    {
        NotEnoughSpace, Overlap, MaxAmountReached, MaxTypeAmountReached, Success
    };

    class Board
    {
    private:
        std::vector<Ship> ships;
        std::map<ShipType, int> shipsCount;
        std::map<Coordinate, ShotHistory> shotHistory;
        int maxShips = 0;

        // Return false if coord is out of board boundaries
        bool IsValidCoordinate(const Coordinate& coord) const;

        // Tries to shoot a ship (if it exists) at given coord
        ShotResult CheckShipsForHit(const Coordinate& coord);

        // Returns vector of ship coordinates
        static std::vector<Coordinate> GetShipCoords(const BoardShip& ship, int shipSize);

        // Returns true if all surrounding cells are empty
        bool AreSurroundingCellsEmpty(const Coordinate& coord);

    public:
        const int xSize = 10;
        const int ySize = 10;

        Board();

        // Tries to shoot at given coord
        ShotResult Fire(const Coordinate& coord);

        ShipPlacementResult PlaceShip(const BoardShip& boardShip);

        ShotHistory GetHistoryAt(const Coordinate& coord) const;

        // Returns true if there is a ship cell on coord
        bool CheckForShip(const Coordinate& coord) const;

        bool CanPlaceShips() const;

        bool CanPlaceShipOfType(const ShipType& type) const;

        int GetMaxShipsCount() const;

        const std::map<Coordinate, ShotHistory>& GetShotHistory() const;

        std::map<ShipType, int> GetAvailableShipsAmount() const;
    };
}
