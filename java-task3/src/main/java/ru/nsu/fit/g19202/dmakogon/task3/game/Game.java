package ru.nsu.fit.g19202.dmakogon.task3.game;

import ru.nsu.fit.g19202.dmakogon.task3.game.records.Record;
import ru.nsu.fit.g19202.dmakogon.task3.game.records.StatsManager;

import java.util.Collection;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.CopyOnWriteArrayList;

public class Game
{
    private final Collection<GameSubscriber> subscribers = new CopyOnWriteArrayList<>();
    private final StatsManager statsManager;
    private GameSettings settings;
    private Timer timer;
    private GameField field;
    private int seconds;
    private GameState gameState = GameState.NOT_STARTED;

    public Game(GameSettings settings, StatsManager statsManager)
    {
        this.settings = settings;
        this.seconds = 0;
        this.field = new GameField(settings.getFieldSizeX(), settings.getFieldSizeY(), settings.getBombsCount());
        this.statsManager = statsManager;
        this.timer = new Timer();
    }

    public List<Record> getStatistics()
    {
        return statsManager.getStatistics();
    }

    public int getSeconds()
    {
        return seconds;
    }

    public void subscribe(GameSubscriber sub)
    {
        subscribers.add(sub);
        sub.onFieldUpdate(this);
    }

    public void unsubscribe(GameSubscriber sub)
    {
        subscribers.removeIf(curSub -> curSub.equals(sub));
    }

    private void notifyFieldUpdate()
    {
        subscribers.forEach(s -> s.onFieldUpdate(this));
    }

    private void notifyTimeUpdate()
    {
        subscribers.forEach(s -> s.onTimeUpdate(this, seconds));
    }

    private void notifyStateChange()
    {
        subscribers.forEach(s -> s.onStateChange(this, gameState));
    }

    private void notifySettingsChange()
    {
        subscribers.forEach(s -> s.onSettingsChange(this, settings));
    }

    public GameSettings getSettings()
    {
        return settings;
    }

    public void setSettings(GameSettings newSettings)
    {
        this.settings = newSettings;
        notifySettingsChange();
    }

    private void onTimerTick()
    {
        this.seconds++;
        notifyTimeUpdate();
    }

    private void startTimer()
    {
        timer.schedule(new TimerTask()
        {
            @Override
            public void run()
            {
                onTimerTick();
            }
        }, 1000, 1000);
    }

    public void openCell(int x, int y)
    {
        if (gameState == GameState.NOT_STARTED)
        {
            field.generate(x, y);
            if (!field.isGenerated())
            {
                return;
            }
            field.openCell(x, y);
            gameState = GameState.STARTED;
            notifyFieldUpdate();
            notifyStateChange();
            startTimer();
        }
        else if (gameState == GameState.STARTED)
        {
            FieldCell.CellType cellType = field.openCell(x, y);
            FieldCell cell = field.getCellAt(x, y);
            boolean isCellOpened = !cell.isClosed();

            notifyFieldUpdate();

            boolean isGameFinished = false;

            if (isCellOpened && cellType == FieldCell.CellType.MINE)
            {
                gameState = GameState.LOST;
                isGameFinished = true;
            }
            else if (0 == field.getClosedEmptyCellsCount())
            {
                gameState = GameState.WON;
                isGameFinished = true;
            }

            if (isGameFinished)
            {
                field.openClosedMines();
                timer.cancel();
                notifyFieldUpdate();
                notifyStateChange();
                notifyTimeUpdate();
            }
        }
    }

    public void storeRecord(String name)
    {
        if (gameState == GameState.WON)
        {
            statsManager.addRecord(name, seconds);
        }
    }

    public int getRemainingMinesCount()
    {
        return field.getRemainingMinesCount();
    }

    public GameState getGameState()
    {
        return gameState;
    }

    public void markCell(int x, int y)
    {
        field.markCell(x, y);
        notifyFieldUpdate();
    }

    public FieldCell getCell(int x, int y)
    {
        return field.getCellAt(x, y);
    }

    public void restart()
    {
        timer.cancel();
        timer = new Timer();
        field = new GameField(settings.getFieldSizeX(), settings.getFieldSizeY(), settings.getBombsCount());
        gameState = GameState.NOT_STARTED;
        seconds = 0;
        notifyFieldUpdate();
        notifyStateChange();
        notifyTimeUpdate();
    }

    public enum GameState
    {NOT_STARTED, STARTED, LOST, WON}
}
