package ru.nsu.fit.g19202.dmakogon.task3.gui;

import javax.swing.*;
import java.awt.*;

public class ResourceLoader
{
    public static Image loadImage(String path, JComponent caller) throws ResourceLoadingException
    {
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        MediaTracker tracker = new MediaTracker(caller);
        Image image = toolkit.createImage(caller.getClass().getResource(path));
        tracker.addImage(image, 0);
        try
        {
            tracker.waitForID(0);
        }
        catch (InterruptedException e)
        {
            throw new ResourceLoadingException("Interrupted", e);
        }
        if (tracker.isErrorID(0))
        {
            throw new ResourceLoadingException("Failed to load image '" + path);
        }
        return image;
    }
}
