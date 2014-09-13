package com.technegames.battlebombs.json;

public interface JsonDeserializer
{
    <T> T deserializeJsonIntoType(String json, Class<T> type);
}