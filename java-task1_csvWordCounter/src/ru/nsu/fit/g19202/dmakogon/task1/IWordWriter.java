package ru.nsu.fit.g19202.dmakogon.task1;

import java.io.IOException;
import java.io.Writer;

public interface IWordWriter
{
    void writeWord(Word word, Writer writer) throws IOException;
}
