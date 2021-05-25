package ru.nsu.fit.g19202.dmakogon.task1;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public class DescendingWordSorter implements IWordSorter
{
    @Override
    public Collection<Word> sortWords(Collection<Word> words)
    {
        List<Word> wordsList = new ArrayList<>(words);
        wordsList.sort((a, b) -> {
            int intCmp = Integer.compare(b.getCount(), a.getCount());
            if (intCmp == 0)
            {
                return a.toString().compareTo(b.toString());
            }
            return intCmp;
        });
        return wordsList;
    }
}
