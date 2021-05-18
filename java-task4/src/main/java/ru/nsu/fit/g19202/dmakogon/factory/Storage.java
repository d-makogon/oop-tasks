package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.Item;

import java.util.ArrayList;
import java.util.List;

public class Storage<T extends Item>
{
    private final Logger logger = LogManager.getLogger();
    private final Object lock = new Object();

    private final int size;
    private final List<T> items;

    private final List<StorageObserver<T>> observers;

    public Storage(int size)
    {
        this.size = size;
        items = new ArrayList<>(size);
        observers = new ArrayList<>();
    }

    public T getItem() throws InterruptedException
    {
        synchronized (lock)
        {
            while (true)
            {
                if (!items.isEmpty())
                {
                    T item = items.remove(0);
                    for (StorageObserver<T> observer : observers)
                    {
                        observer.itemRemoved(this, item);
                    }
                    lock.notifyAll();
                    return item;
                }
                else
                {
                    lock.wait();
                }
            }
        }
    }

    public void putItem(T item) throws InterruptedException
    {
        synchronized (lock)
        {
            while (true)
            {
                // if storage is full
                if (items.size() == size)
                {
                    lock.wait();
                }
                else
                {
                    items.add(item);
                    for (StorageObserver<T> observer : observers)
                    {
                        observer.itemAdded(this, item);
                    }
                    lock.notifyAll();
                    return;
                }
            }
        }
    }

    public boolean isEmpty()
    {
        synchronized (lock)
        {
            return items.isEmpty();
        }
    }

    public int getCurrentSize()
    {
        synchronized (lock)
        {
            return items.size();
        }
    }

    public void addObserver(StorageObserver<T> observer)
    {
        synchronized (lock)
        {
            observers.add(observer);
        }
    }

    public void removeObserver(StorageObserver<T> observer)
    {
        synchronized (lock)
        {
            observers.remove(observer);
        }
    }
}
