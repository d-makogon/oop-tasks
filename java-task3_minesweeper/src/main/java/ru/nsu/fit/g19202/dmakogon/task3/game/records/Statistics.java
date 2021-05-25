package ru.nsu.fit.g19202.dmakogon.task3.game.records;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Statistics
{
    private final List<Record> records = new ArrayList<>();
    private boolean isSorted = false;

    public void addRecord(Record record)
    {
        isSorted = false;
        records.add(record);
    }

    public List<Record> getRecords()
    {
        if (!isSorted)
        {
            records.sort(Comparator.comparingInt(Record::getSeconds));
            for (int i = 0; i < records.size(); i++)
            {
                records.get(i).setPlace(i + 1);
            }
        }
        isSorted = true;
        return Collections.unmodifiableList(records);
    }
}
