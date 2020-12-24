#pragma once

#include <vector>
#include <map>

#include <shipTypes.h>
#include <board.h>

namespace bs
{
    class PlayerController
    {
    public:
        virtual BoardShip GetShipPlaceInfo(const bs::Board& board) = 0;

        virtual void ReceiveShipPlaceResult(const ShipPlacementResult& result) = 0;

        virtual Coordinate
        GetShootPosition(const bs::Board& enemyBoard) = 0;

        virtual void ReceiveAllyShotResult(const ShotResult& shotResult) = 0;

        virtual void ReceiveEnemyShotResult(const bs::Coordinate& coord, const ShotResult& shotResult) = 0;

        virtual bool GetYesOrNo(const std::string& msg) = 0;

        virtual bool IsHuman() const = 0;

        virtual ~PlayerController() = default;
    };
}
