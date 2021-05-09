package ru.nsu.fit.g19202.dmakogon.task3.game.records;

public class Record
{
    private final String name;
    private final int seconds;
    private int place;

    public Record(String name, int seconds, int place)
    {
        this.name = name;
        this.seconds = seconds;
        this.place = place;
    }

    public int getPlace()
    {
        return place;
    }

    public void setPlace(int place)
    {
        this.place = place;
    }

    public int getSeconds()
    {
        return seconds;
    }

    public String getName()
    {
        return name;
    }
}
