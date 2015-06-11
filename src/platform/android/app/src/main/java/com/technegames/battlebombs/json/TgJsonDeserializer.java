package com.technegames.battlebombs.json;

import com.google.gson.Gson;

public final class TgJsonDeserializer implements JsonDeserializer
{
    @Override
    public <T> T deserializeJsonIntoType(String json, Class<T> type)
    {
        Gson gson = new Gson();
        T jsonObject = gson.fromJson(json, type);

        return jsonObject;
    }
}