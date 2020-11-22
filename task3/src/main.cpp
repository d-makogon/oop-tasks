#include <memory>

#include "graphics/console/consoleView.h"
#include "graphics/sfml/sfmlView.h"
#include "playerControllers/playerController.h"
#include "playerControllers/consolePlayerController.h"
#include "playerControllers/randomPlayerController.h"
#include "game/gameLogic.h"

int main(int argc, char** argv)
{
    // todo: factories
    auto p1 = std::make_unique<bs::ConsolePlayerController>();
    auto p2 = std::make_unique<bs::RandomPlayerController>();
    auto logic = std::make_unique<bs::GameLogic>();
    std::unique_ptr<bs::GameView> view = std::make_unique<bs::ConsoleView>(std::move(logic), std::move(p1),
                                                                           std::move(p2));
    view->Do(true, false);
    return 0;
}