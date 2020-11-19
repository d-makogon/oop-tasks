#include "graphics/console/consoleView.h"
#include "playerControllers/playerController.h"
#include "playerControllers/consolePlayerController.h"
#include "playerControllers/randomPlayerController.h"
#include "game/gameLogic.h"

int main(int argc, char** argv)
{
    bs::PlayerController* p1 = new bs::RandomPlayerController();
    bs::PlayerController* p2 = new bs::RandomPlayerController();
    bs::GameLogic logic;
    bs::ConsoleView view(logic, p2, p1);
    view.Do();

    delete p1;
    delete p2;

    return 0;
}