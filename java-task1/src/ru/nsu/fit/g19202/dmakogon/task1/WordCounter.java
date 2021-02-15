package ru.nsu.fit.g19202.dmakogon.task1;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

public class WordCounter
{
    private final Map<String, Word> wordsMap;
    private int totalWordsCount = 0;

    public WordCounter()
    {
        wordsMap = new HashMap<>();
    }

    public void countWord(String wordString)
    {
        Word wordInMap = wordsMap.get(wordString);
        if (wordInMap == null)
        {
            wordsMap.put(wordString, new Word(wordString, 1));
        }
        else
        {
            wordInMap.increaseCount();
        }
        totalWordsCount++;
    }

    public Collection<Word> getWords()
    {
        Collection<Word> words = wordsMap.values();
        words.forEach(word -> word.setPercentage(getWordPercentage(word)));
        return words;
    }

    private double getWordPercentage(Word word)
    {
        return (totalWordsCount == 0) ? 0 : (100 * (double) word.getCount() / totalWordsCount);
    }
}
