package ru.nsu.fit.g19202.dmakogon.task3.game;

import ru.nsu.fit.g19202.dmakogon.task3.game.records.Record;

import java.util.ArrayList;
import java.util.List;

public class GameController
{
    private final Game game;

    public GameController(Game game)
    {
        this.game = game;
    }

    public Game getGame()
    {
        return game;
    }

    public void restartGame()
    {
        game.restart();
    }

    public Game.GameState getGameState()
    {
        return game.getGameState();
    }

    public FieldCell getCell(int x, int y)
    {
        return game.getCell(x, y);
    }

    public void openCell(int x, int y)
    {
        game.openCell(x, y);
    }

    public void markCell(int x, int y)
    {
        game.markCell(x, y);
    }

    public GameSettings getGameSettings()
    {
        return game.getSettings();
    }

    public void setGameSettings(GameSettings newSettings)
    {
        game.setSettings(newSettings);
    }

    public List<GameSettings> getAvailableSettings()
    {
        try
        {
            return GameSettings.readSettings();
        }
        catch (InvalidSettingsException e)
        {
            return new ArrayList<>();
        }
    }

    public void storeResult(String name)
    {
        game.storeRecord(name);
    }

    public List<Record> getStatistics()
    {
        return game.getStatistics();
    }
}
