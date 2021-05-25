package ru.nsu.fit.g19202.dmakogon.task3;

import ru.nsu.fit.g19202.dmakogon.task3.game.Game;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameController;
import ru.nsu.fit.g19202.dmakogon.task3.game.GameSettings;
import ru.nsu.fit.g19202.dmakogon.task3.game.InvalidSettingsException;
import ru.nsu.fit.g19202.dmakogon.task3.game.records.StatsManager;
import ru.nsu.fit.g19202.dmakogon.task3.gui.GameFrame;
import ru.nsu.fit.g19202.dmakogon.task3.gui.ResourceLoadingException;

import java.io.IOException;
import java.util.List;

public class Main
{
    public static void main(String[] args) throws IOException
    {
        StatsManager statsManager = new StatsManager();
        List<GameSettings> settings;
        try
        {
            settings = GameSettings.readSettings();
        }
        catch (InvalidSettingsException e)
        {
            System.err.println(e.getLocalizedMessage());
            return;
        }
        Game game = new Game(settings.get(0), statsManager);

        GameController gameController = new GameController(game, settings);

        View gameView;
        try
        {
            gameView = new GameFrame(gameController);
        }
        catch (ResourceLoadingException e)
        {
            System.err.println(e.getLocalizedMessage());
            return;
        }

        gameView.setExitAction(() -> {
            try
            {
                statsManager.writeStatistics();
                System.exit(0);
            }
            catch (IOException e)
            {
                e.printStackTrace();
                System.exit(1);
            }
        });

        gameView.run();
    }
}
