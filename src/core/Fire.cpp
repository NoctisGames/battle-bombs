//
//  Fire.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 6/21/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Fire.h"
#include "Vector2D.h"
#include "Rectangle.h"

Fire::Fire(Fire_Type type, int gridX, int gridY, float angle, float width, float height) : GridGameObject(gridX, gridY, width, height, angle)
{
    m_fireType = type;
    m_isExhausted = false;
}

void Fire::nextFrame(short explosionPowerRemaining)
{
    switch (m_fireType)
    {
        case CORE_PART_1:
            m_fireType = CORE_PART_2;
            break;
        case CORE_PART_2:
            m_fireType = CORE_PART_3;
            break;
        case CORE_PART_3:
            m_fireType = explosionPowerRemaining == 1 ? CORE_PART_4_POW_1 : CORE_PART_4_POW_2;
            break;
        case CORE_PART_4_POW_1:
            m_fireType = CORE_END;
            break;
        case CORE_PART_4_POW_2:
            m_fireType = CORE_PART_5_POW_2;
            break;
        case CORE_PART_5_POW_2:
            m_fireType = explosionPowerRemaining == 0 ? CORE_PART_6_POW_2 : m_fireType;
            break;
        case CORE_PART_6_POW_2:
            m_fireType = CORE_END;
            break;
        case CORE_END:
            m_isExhausted = true;
            break;
        case HEAD_PART_1:
            m_fireType = HEAD_PART_2;
            break;
        case HEAD_PART_2:
            m_fireType = HEAD_PART_3;
            break;
        case HEAD_PART_3:
            m_fireType = explosionPowerRemaining == 1 ? HEAD_PART_4_POW_1 : FAT_NECK;
            break;
        case HEAD_PART_4_POW_1:
            m_isExhausted = true;
            break;
        case HEAD_PART_4_POW_2:
            m_fireType = THIN_NECK;
            break;
        case HEAD_PART_5_POW_2:
            m_fireType = explosionPowerRemaining == 0 ? HEAD_PART_6_POW_2 : THIN_NECK;
            break;
        case HEAD_PART_6_POW_2:
            m_isExhausted = true;
            break;
        case EDGE_FROM_CORE_PART_1:
            m_fireType = EDGE_FROM_CORE_PART_2;
            break;
        case EDGE_FROM_CORE_PART_2:
            m_fireType = EDGE_FROM_CORE_PART_3;
            break;
        case EDGE_FROM_CORE_PART_3:
            m_fireType = explosionPowerRemaining == 1 ? EDGE_FROM_CORE_PART_4_POW_1 : EDGE_FROM_CORE_PART_4_POW_2;
            break;
        case EDGE_FROM_CORE_PART_4_POW_1:
            m_isExhausted = true;
            break;
        case EDGE_FROM_CORE_PART_4_POW_2:
            m_fireType = EDGE_FROM_CORE_PART_5_POW_2;
            break;
        case EDGE_FROM_CORE_PART_5_POW_2:
            m_isExhausted = true;
            break;
        case EDGE_FROM_FAT_NECK:
            m_fireType = EDGE_FROM_THIN_NECK;
            break;
        case EDGE_FROM_THIN_NECK:
            m_isExhausted = explosionPowerRemaining == 0;
            break;
        case FAT_NECK:
            m_fireType = BODY;
            break;
        case THIN_NECK:
            m_fireType = BODY;
            break;
        case BODY:
            m_fireType = explosionPowerRemaining == 0 ? NECK_AND_BODY_END : BODY;
            break;
        case NECK_AND_BODY_END:
            m_isExhausted = true;
            break;
        default:
            break;
    }
}

bool Fire::isExhausted()
{
    return m_isExhausted;
}

Fire_Type Fire::getFireType()
{
    return m_fireType;
}