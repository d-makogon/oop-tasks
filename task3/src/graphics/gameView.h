#pragma once

#include <utility>

#include "../game/board/board.h"
#include "../game/gameLogic.h"
#include "../playerControllers/playerController.h"

namespace bs
{
    class GameView
    {
    protected:
        std::unique_ptr<GameLogic> logic;

        std::unique_ptr<PlayerController> pc1;
        std::unique_ptr<PlayerController> pc2;

        void PlaceShip(PlayerController& pc, const Board& board);

        void Shoot(PlayerController& pc, const Board& board);

    public:
        explicit GameView(std::unique_ptr<GameLogic> logic, std::unique_ptr<PlayerController> pc1,
                          std::unique_ptr<PlayerController> pc2)
                : logic(std::move(logic)),
                  pc1(std::move(pc1)),
                  pc2(std::move(pc2)) {}

        virtual void Do(bool revealPlayer1, bool revealPlayer2) = 0;

        ~GameView() = default;
    };
}