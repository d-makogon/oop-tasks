#pragma once

#include <utility>

#include "../game/board/board.h"
#include "../game/gameLogic.h"

namespace bs
{
    class GameView
    {
    protected:
        // todo: use ptr
        GameLogic logic;

        PlayerController* pc1;
        PlayerController* pc2;
    public:
        explicit GameView(GameLogic logic, PlayerController* pc1, PlayerController* pc2) : logic(std::move(logic)),
                                                                                           pc1(pc1),
                                                                                           pc2(pc2) {}

        virtual void Do() = 0;

        ~GameView() = default;
    };
}