package ru.nsu.fit.g19202.dmakogon.chat;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class NonCloseableInputStream extends InputStream
{
    private final InputStream underlyingInputStream;

    public NonCloseableInputStream(InputStream underlyingInputStream)
    {
        this.underlyingInputStream = underlyingInputStream;
    }

    @Override
    public int read() throws IOException
    {
        return underlyingInputStream.read();
    }

    @Override
    public int read(byte[] b) throws IOException
    {
        return underlyingInputStream.read(b);
    }

    @Override
    public int read(byte[] b, int off, int len) throws IOException
    {
        return underlyingInputStream.read(b, off, len);
    }

    @Override
    public byte[] readAllBytes() throws IOException
    {
        return underlyingInputStream.readAllBytes();
    }

    @Override
    public byte[] readNBytes(int len) throws IOException
    {
        return underlyingInputStream.readNBytes(len);
    }

    @Override
    public int readNBytes(byte[] b, int off, int len) throws IOException
    {
        return underlyingInputStream.readNBytes(b, off, len);
    }

    @Override
    public long skip(long n) throws IOException
    {
        return underlyingInputStream.skip(n);
    }

    @Override
    public void skipNBytes(long n) throws IOException
    {
        underlyingInputStream.skipNBytes(n);
    }

    @Override
    public int available() throws IOException
    {
        return underlyingInputStream.available();
    }

    @Override
    public void close()
    {
        // do nothing
    }

    @Override
    public synchronized void mark(int readlimit)
    {
        underlyingInputStream.mark(readlimit);
    }

    @Override
    public synchronized void reset() throws IOException
    {
        underlyingInputStream.reset();
    }

    @Override
    public boolean markSupported()
    {
        return underlyingInputStream.markSupported();
    }

    @Override
    public long transferTo(OutputStream out) throws IOException
    {
        return underlyingInputStream.transferTo(out);
    }
}
