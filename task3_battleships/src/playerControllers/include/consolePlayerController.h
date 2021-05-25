#pragma once

#include "playerController.h"

namespace bs
{
    class ConsolePlayerController : public PlayerController
    {
    public:
        bs::BoardShip GetShipPlaceInfo(const bs::Board& board) override;

        void ReceiveShipPlaceResult(const ShipPlacementResult& result) override;

        Coordinate GetShootPosition(const bs::Board& enemyBoard) override;

        void ReceiveEnemyShotResult(const bs::Coordinate& coord, const ShotResult& shotResult) override;

        bool GetYesOrNo(const std::string& msg) override;

        bool IsHuman() const override { return true; }

        void ReceiveAllyShotResult(const ShotResult& shotResult) override;
    };
}
