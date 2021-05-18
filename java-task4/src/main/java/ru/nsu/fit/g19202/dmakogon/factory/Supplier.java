package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.Item;
import ru.nsu.fit.g19202.dmakogon.factory.items.ItemsFactory;

import java.util.concurrent.atomic.AtomicInteger;

public class Supplier<T extends Item> extends Thread
{
    private final Logger logger = LogManager.getLogger();

    private final Storage<T> storage;
    private final AtomicInteger itemSupplyTime;
    private final ItemsFactory<T> itemsFactory;

    public Supplier(String name, Storage<T> storage, int itemSupplyTime, ItemsFactory<T> itemsFactory)
    {
        super(name);
        this.storage = storage;
        this.itemSupplyTime = new AtomicInteger(itemSupplyTime);
        this.itemsFactory = itemsFactory;
    }

    @Override
    public void run()
    {
        while (true)
        {
            try
            {
                sleep(itemSupplyTime.get());
                T item = itemsFactory.getNewItem();
                storage.putItem(item);
                logger.info(getName() + " put in storage " + item);
            }
            catch (InterruptedException e)
            {
                logger.warn(getName() + " interrupted");
                return;
            }
        }
    }

    public void setItemSupplyTime(int itemSupplyTime)
    {
        this.itemSupplyTime.set(itemSupplyTime);
    }
}
