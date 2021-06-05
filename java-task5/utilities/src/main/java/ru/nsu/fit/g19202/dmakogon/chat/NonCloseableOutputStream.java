package ru.nsu.fit.g19202.dmakogon.chat;

import java.io.IOException;
import java.io.OutputStream;

public class NonCloseableOutputStream extends OutputStream
{
    private final OutputStream underlyingOutputStream;

    public NonCloseableOutputStream(OutputStream underlyingOutputStream)
    {
        this.underlyingOutputStream = underlyingOutputStream;
    }

    @Override
    public void write(byte[] b) throws IOException
    {
        underlyingOutputStream.write(b);
    }

    @Override
    public void write(byte[] b, int off, int len) throws IOException
    {
        underlyingOutputStream.write(b, off, len);
    }

    @Override
    public void flush() throws IOException
    {
        underlyingOutputStream.flush();
    }

    @Override
    public void close()
    {
        // do nothing
    }

    @Override
    public void write(int b) throws IOException
    {
        underlyingOutputStream.write(b);
    }
}
