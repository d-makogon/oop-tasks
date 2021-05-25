package ru.nsu.fit.g19202.dmakogon.task3.game.records;

import com.google.gson.Gson;

import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;

public class StatsManager
{
    private static final String RECORDS_FILE = "records.json";
    private final Gson gson = new Gson();
    private Statistics statistics;

    public StatsManager()
    {
        try
        {
            readStatistics();
        }
        catch (IOException e)
        {
            statistics = new Statistics();
        }
    }

    private void readStatistics() throws IOException
    {
        try (FileInputStream fis = new FileInputStream(RECORDS_FILE))
        {
            InputStreamReader reader = new InputStreamReader(fis);
            statistics = gson.fromJson(reader, Statistics.class);
            if (statistics == null)
            {
                statistics = new Statistics();
            }
        }
    }

    public List<Record> getStatistics()
    {
        return statistics.getRecords();
    }

    public void writeStatistics() throws IOException
    {
        try (FileWriter writer = new FileWriter(RECORDS_FILE))
        {
            gson.toJson(statistics, Statistics.class, writer);
        }
    }

    public void addRecord(String name, int seconds)
    {
        statistics.addRecord(new Record((name != null) ? name : "", seconds, 0));
    }
}
