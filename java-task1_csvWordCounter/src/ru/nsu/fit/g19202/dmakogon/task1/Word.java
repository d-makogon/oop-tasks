package ru.nsu.fit.g19202.dmakogon.task1;

import java.util.Objects;

public class Word implements Comparable<Word>
{
    private final String wordStr;
    private int count;
    private double percentage;

    public Word(String word)
    {
        this.wordStr = word;
        this.count = 1;
        this.percentage = 0;
    }

    public Word(String word, int count)
    {
        this.wordStr = word;
        this.count = count;
        this.percentage = 0;
    }

    public void setCount(int count)
    {
        this.count = count;
    }

    public void increaseCount()
    {
        count++;
    }

    public int getCount()
    {
        return count;
    }

    public double getPercentage()
    {
        return percentage;
    }

    public void setPercentage(double percentage)
    {
        this.percentage = percentage;
    }

    @Override
    public String toString()
    {
        return wordStr;
    }

    @Override
    public boolean equals(Object o)
    {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Word word = (Word) o;
        return wordStr.equals(word.wordStr);
    }

    @Override
    public int hashCode()
    {
        return Objects.hash(wordStr);
    }

    @Override
    public int compareTo(Word o)
    {
        return Integer.compare(count, o.count);
    }
}
