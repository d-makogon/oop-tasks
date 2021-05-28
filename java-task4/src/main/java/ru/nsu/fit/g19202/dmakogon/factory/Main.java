package ru.nsu.fit.g19202.dmakogon.factory;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.factory.items.AccessoriesFactory;
import ru.nsu.fit.g19202.dmakogon.factory.items.BodiesFactory;
import ru.nsu.fit.g19202.dmakogon.factory.items.MotorsFactory;
import ru.nsu.fit.g19202.dmakogon.factory.view.SwingCarFactoryView;

import java.io.IOException;

public class Main
{
    public static void main(String[] args) throws IOException
    {
        FactorySettings settings = FactorySettings.readSettings();
        CarFactory carFactory = new CarFactory(settings, new BodiesFactory(), new MotorsFactory(),
                new AccessoriesFactory());
        SwingCarFactoryView view = new SwingCarFactoryView(carFactory);
        settings.addObserver(view);
        settings.addObserver(carFactory);
        carFactory.addObserver(view);
        carFactory.start();
        view.run();

        Runtime.getRuntime().addShutdownHook(new Thread(carFactory::shutdown));
    }
}
