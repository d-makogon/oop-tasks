package ru.nsu.fit.g19202.dmakogon.chat.server;

import java.io.IOException;

public class Main
{
    private static final int DEFAULT_PORT = 4000;

    public static void main(String[] args) throws IOException
    {
        int port = DEFAULT_PORT;
        if (args.length > 0)
        {
            if (args.length > 1)
            {
                System.err.println("Unexpected parameters. Usage: server [port]");
                return;
            }
            try
            {
                port = Integer.parseInt(args[0]);
            }
            catch (NumberFormatException e)
            {
                System.err.println(args[0] + ": " + e.getLocalizedMessage());
                return;
            }
        }
        ChatServer server = new ChatServer(port);
        Thread thread = new Thread(server);
        thread.start();
    }
}
