package com.technegames.battlebombs.json;

import com.google.gson.Gson;

public final class TgJsonSerializer implements JsonSerializer
{
    @Override
    public String serializeJsonObject(Object jsonRequestObject)
    {
        Gson gson = new Gson();
        String json = gson.toJson(jsonRequestObject);

        return json;
    }
}