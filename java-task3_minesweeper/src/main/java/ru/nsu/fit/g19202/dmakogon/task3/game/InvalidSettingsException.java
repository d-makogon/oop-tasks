package ru.nsu.fit.g19202.dmakogon.task3.game;

public class InvalidSettingsException extends Exception
{
    public InvalidSettingsException()
    {
    }

    public InvalidSettingsException(String message)
    {
        super(message);
    }

    public InvalidSettingsException(String message, Throwable cause)
    {
        super(message, cause);
    }

    public InvalidSettingsException(Throwable cause)
    {
        super(cause);
    }

    public InvalidSettingsException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace)
    {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
