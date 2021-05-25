package ru.nsu.fit.g19202.dmakogon.task1;

import java.io.*;

public class Main
{
    public static void main(String[] args)
    {
        if (args.length < 1)
        {
            System.err.println("Too few arguments");
            return;
        }

        if (args.length > 2)
        {
            System.err.println("Too many arguments (maximum 2)");
            return;
        }

        InputStream inStream = null;
        OutputStream outStream = System.out;

        if (args.length > 1)
        {
            try
            {
                outStream = new FileOutputStream(args[1]);
            }
            catch (IOException e)
            {
                System.err.println("Error opening file " + args[1] + ": " + e.getLocalizedMessage());
            }
        }

        WordCounter counter = new WordCounter();

        try (WordReader reader = new WordReader(new FileInputStream(args[0])))
        {
            String word = reader.nextWord();
            while (word != null)
            {
                counter.countWord(word);
                word = reader.nextWord();
            }
        }
        catch (IOException e)
        {
            System.err.println("Error while reading file: " + e.getLocalizedMessage());
        }

        var sortedWords = new DescendingWordSorter().sortWords(counter.getWords());

        try (StatsWriter writer = new StatsWriter(sortedWords, outStream))
        {
            writer.write(new CSVWordWriter());
        }
        catch (IOException e)
        {
            System.err.println("Error while writing to file: " + e.getLocalizedMessage());
        }
    }
}
