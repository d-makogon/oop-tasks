#pragma once

#include <SFML/Graphics.hpp>

#include "../gameView.h"

namespace bs
{
    class SFMLView : public GameView
    {
    public:
        explicit SFMLView(std::unique_ptr<GameLogic> logic, std::unique_ptr<PlayerController> pc1,
                          std::unique_ptr<PlayerController> pc2) : GameView(std::move(logic),
                                                                            std::move(pc1),
                                                                            std::move(pc2)) {}

        void Do(bool revealPlayer1, bool revealPlayer2) override;

        ~SFMLView();
    };
}