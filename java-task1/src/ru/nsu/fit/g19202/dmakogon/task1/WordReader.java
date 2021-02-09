package ru.nsu.fit.g19202.dmakogon.task1;

import java.io.*;

public class WordReader implements AutoCloseable
{
    private final Reader reader;

    public WordReader(InputStream iStream)
    {
        reader = new BufferedReader(new InputStreamReader(iStream));
    }

    public void close() throws IOException
    {
        reader.close();
    }

    public String nextWord() throws IOException
    {
        var sBuilder = new StringBuilder();
        int curChar = reader.read();

        // skip leading non-word symbols in the stream
        while (!Character.isLetterOrDigit(curChar))
        {
            // if EOF
            if (curChar == -1) return null;
            curChar = reader.read();
        }

        // read word
        while (Character.isLetterOrDigit(curChar))
        {
            sBuilder.appendCodePoint(curChar);
            curChar = reader.read();
        }
        return sBuilder.toString();
    }
}
