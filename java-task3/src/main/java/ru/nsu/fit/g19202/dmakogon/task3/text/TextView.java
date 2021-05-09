package ru.nsu.fit.g19202.dmakogon.task3.text;

import ru.nsu.fit.g19202.dmakogon.task3.View;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameController;

public class TextView implements View
{
    private final GameController gameController;

    public TextView(GameController gameController)
    {
        this.gameController = gameController;
    }

    @Override
    public void run()
    {

    }

    @Override
    public void setExitAction(Runnable action)
    {

    }
}
