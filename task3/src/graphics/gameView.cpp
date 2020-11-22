#include "gameView.h"

void bs::GameView::PlaceShip(PlayerController& pc, const Board& board)
{
    std::vector<ShipType> availableTypes;
    for (auto&[type, count] : board.GetAvailableShipsAmount())
    {
        if (count > 0) availableTypes.push_back(type);
    }


    static std::vector<ShipDirection> availableDirs;
    if (availableDirs.empty())
    {
        availableDirs.reserve(ShipDirsNames.size());
        for (auto&[dir, name] : ShipDirsNames)
        {
            availableDirs.push_back(dir);
        }
    }

    bs::BoardShip bs = pc.GetShipPlaceInfo(availableTypes, availableDirs, board.xSize - 1, board.ySize - 1);

    // todo: remove this
    // Console::PrintFormatted("Chosen %s type, %s dir, (%c, %d) cell\n",
    //                         ShipNames.at(bs.GetType()),
    //                         ShipDirsNames.at(bs.GetDirection()),
    //                         Console::CoordToLetter(bs.GetCoordinate().GetX()),
    //                         bs.GetCoordinate().GetY());

    pc.ReceiveShipPlaceResult(logic->PlaceShip(bs));
}

void bs::GameView::Shoot(PlayerController& pc, const Board& board)
{
    auto c = pc.GetShootPosition(board.xSize - 1, board.ySize - 1, board.GetShotHistory());
    pc.ReceiveShotResult(logic->Shoot(c));
}
