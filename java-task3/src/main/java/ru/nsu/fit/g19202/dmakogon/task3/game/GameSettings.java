package ru.nsu.fit.g19202.dmakogon.task3.game;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

public class GameSettings
{
    private static final String SETTINGS_FILE = "/settings/settings.properties";

    private static List<GameSettings> settingsList;
    private final int fieldSizeX;
    private final int fieldSizeY;
    private final int bombsCount;
    private final String name;

    public GameSettings(String name, int fieldSizeX, int fieldSizeY, int bombsCount)
    {
        this.name = name;
        this.fieldSizeX = fieldSizeX;
        this.fieldSizeY = fieldSizeY;
        this.bombsCount = bombsCount;
    }

    public static List<GameSettings> readSettings() throws InvalidSettingsException, IOException
    {
        if (settingsList != null)
        {
            return settingsList;
        }

        settingsList = new ArrayList<>();

        try (InputStream inputStream = GameSettings.class.getResourceAsStream(SETTINGS_FILE))
        {
            if (inputStream == null)
            {
                throw new FileNotFoundException(SETTINGS_FILE + "not found.");
            }

            PropertyResourceBundle bundle = new PropertyResourceBundle(inputStream);

            var keys = bundle.getKeys().asIterator();
            while (keys.hasNext())
            {
                String key = keys.next();
                String value = bundle.getString(key);
                String[] split = value.split("(,|, )");
                if (split.length != 3)
                {
                    throw new InvalidSettingsException(value + " doesn't match pattern 'sizeX, sizeY, minesCount'");
                }
                int sizeX = Integer.parseInt(split[0]);
                int sizeY = Integer.parseInt(split[1]);
                int minesCount = Integer.parseInt(split[2]);

                settingsList.add(new GameSettings(key, sizeX, sizeY, minesCount));
            }
        }

        settingsList.sort(Comparator.comparingInt(s -> s.bombsCount));

        return settingsList;
    }

    public int getFieldSizeX()
    {
        return fieldSizeX;
    }

    public int getFieldSizeY()
    {
        return fieldSizeY;
    }

    public int getBombsCount()
    {
        return bombsCount;
    }

    public String getName()
    {
        return name;
    }

    @Override
    public String toString()
    {
        return String.format("%s (%d, %d, %d)", name, fieldSizeX, fieldSizeY, bombsCount);
    }
}
