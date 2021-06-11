package ru.nsu.fit.g19202.dmakogon.chat;

import java.util.ArrayList;
import java.util.List;

public class Message
{
    private final Type type;
    private String clientName = "";
    private int clientId = 0;
    private String errorMessage = "";
    private List<String> usersNamesList = new ArrayList<>();
    private String message = "";

    public Message()
    {
        type = Type.EMPTY_SUCCESS;
    }

    private Message(Type type) {this.type = type;}

    public static Message createLoginMessage(String clientName)
    {
        Message msg = new Message(Type.LOGIN);
        msg.clientName = clientName;
        return msg;
    }

    public static Message createErrorMessage(String errorMessage)
    {
        Message msg = new Message(Type.ERROR);
        msg.errorMessage = errorMessage;
        return msg;
    }

    public static Message createLoginSuccessMessage(int clientId)
    {
        Message msg = new Message(Type.LOGIN_SUCCESS);
        msg.clientId = clientId;
        return msg;
    }

    public static Message createListMessage(int clientId)
    {
        Message msg = new Message(Type.LIST);
        msg.clientId = clientId;
        return msg;
    }

    public static Message createListSuccessMessage(List<String> clientNames)
    {
        Message msg = new Message(Type.LIST_SUCCESS);
        msg.usersNamesList = clientNames;
        return msg;
    }

    public static Message createOutTextMessage(int id, String text)
    {
        Message msg = new Message(Type.MESSAGE_OUT);
        msg.clientId = id;
        msg.message = text;
        return msg;
    }

    public static Message createEmptySuccessMessage()
    {
        return new Message(Type.EMPTY_SUCCESS);
    }

    public static Message createInTextMessage(String clientName, String text)
    {
        Message msg = new Message(Type.MESSAGE_IN);
        msg.clientName = clientName;
        msg.message = text;
        return msg;
    }

    public static Message createLogoutMessage(int clientId)
    {
        Message msg = new Message(Type.LOGOUT);
        msg.clientId = clientId;
        return msg;
    }

    public static Message createUserLoginMessage(String name)
    {
        Message msg = new Message(Type.USER_LOGIN_EVENT);
        msg.clientName = name;
        return msg;
    }

    public static Message createUserLogoutMessage(String name)
    {
        Message msg = new Message(Type.USER_LOGOUT_EVENT);
        msg.clientName = name;
        return msg;
    }

    public int getClientId()
    {
        return clientId;
    }

    public Type getType()
    {
        return type;
    }

    public String getClientName()
    {
        return clientName;
    }

    public String getErrorMessage()
    {
        return errorMessage;
    }

    public List<String> getUsersList()
    {
        return usersNamesList;
    }

    public String getMessage()
    {
        return message;
    }

    public enum Type
    {
        LOGIN, ERROR, LOGIN_SUCCESS, LIST, LIST_SUCCESS, MESSAGE_OUT, MESSAGE_IN, EMPTY_SUCCESS, LOGOUT,
        USER_LOGIN_EVENT, USER_LOGOUT_EVENT
    }
}
