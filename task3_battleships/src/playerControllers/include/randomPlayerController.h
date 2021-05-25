#pragma once

#include "playerController.h"
#include <random.h>

namespace bs
{
    class RandomPlayerController : public PlayerController
    {
    private:
        Random random;

    public:
        RandomPlayerController() : random() {}

        BoardShip GetShipPlaceInfo(const bs::Board& board) override;

        void ReceiveShipPlaceResult(const ShipPlacementResult& result) override {}

        Coordinate GetShootPosition(const bs::Board& enemyBoard) override;

        bool IsHuman() const override { return false; }

        void ReceiveAllyShotResult(const ShotResult& shotResult) override {}

        bool GetYesOrNo(const std::string& msg) override { return true; }

        void ReceiveEnemyShotResult(const bs::Coordinate& coord, const ShotResult& shotResult) override {}
    };
}
