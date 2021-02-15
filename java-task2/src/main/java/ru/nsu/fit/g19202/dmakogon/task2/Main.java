package ru.nsu.fit.g19202.dmakogon.task2;

import ru.nsu.fit.g19202.dmakogon.task2.calc.StackCalculator;

import java.io.*;
import java.util.logging.Logger;

public class Main
{
    private static final Logger logger = Logger.getLogger(Main.class.getName());

    public static void main(String[] args)
    {
        // String s = "DEFINE a 3.1415\nPUSH b\nPUSH 5\nPUSH a\n*\nPRINT";
        // InputStream fis = new ByteArrayInputStream(s.getBytes(StandardCharsets.UTF_8));

        if (args.length > 2)
        {
            logger.warning("Too many arguments given. Exiting.");
            // System.err.println("Too many arguments. Excepted 0 or 1.");
            return;
        }

        try
        {
            StackCalculator calc = new StackCalculator(getInputStream(args), getOutputStream(args));
            calc.execute();
        }
        catch (IOException e)
        {
            logger.warning(e.getMessage());
            // System.err.println(e.getLocalizedMessage());
        }
    }

    private static InputStream getInputStream(String[] args) throws FileNotFoundException
    {
        if (args.length == 0)
        {
            logger.info("Using System.in as input stream.");
            return System.in;
        }
        logger.info("Using " + args[0] + " as input stream.");
        return new FileInputStream(args[0]);
    }

    private static OutputStream getOutputStream(String[] args) throws FileNotFoundException
    {
        if (args.length < 2)
        {
            logger.info("Using System.out as output stream.");
            return System.out;
        }
        logger.info("Using " + args[0] + " as output stream.");
        return new FileOutputStream(args[1]);
    }
}
