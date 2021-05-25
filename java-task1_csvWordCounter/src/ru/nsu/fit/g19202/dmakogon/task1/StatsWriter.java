package ru.nsu.fit.g19202.dmakogon.task1;

import java.io.*;
import java.util.Collection;

public class StatsWriter implements AutoCloseable
{
    private final Collection<Word> words;
    private final Writer writer;

    public StatsWriter(Collection<Word> words, OutputStream oStream)
    {
        this.words = words;
        writer = new BufferedWriter(new OutputStreamWriter(oStream));
    }

    public void close() throws IOException
    {
        writer.close();
    }

    public void write(IWordWriter wordWriter) throws IOException
    {
        for (Word word : words)
        {
            wordWriter.writeWord(word, writer);
        }

        writer.flush();
    }
}
