package ru.nsu.fit.g19202.dmakogon.task1;

import java.io.IOException;
import java.io.Writer;
import java.util.Locale;

public class CSVWordWriter implements IWordWriter
{
    @Override
    public void writeWord(Word word, Writer writer) throws IOException
    {
        // US locale to print floating point numbers with '.'
        writer.write(String.format(Locale.US, "%s,%d,%.3f%n", word.toString(), word.getCount(), word.getPercentage()));
    }
}
