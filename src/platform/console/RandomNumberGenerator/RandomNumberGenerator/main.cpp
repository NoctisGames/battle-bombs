//
//  main.cpp
//  RandomNumberGenerator
//
//  Created by Stephen Gowen on 10/13/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

bool isIntAlreadyInsideVector(std::vector<int> &coords, int coordinate)
{
    for (std::vector<int> ::iterator itr = coords.begin(); itr != coords.end(); itr++)
    {
        if((*itr) == coordinate)
        {
            return true;
        }
    }
    
    return false;
}

int main(int argc, const char * argv[])
{
    srand((int)time(NULL));
    
    std::vector<int> coords;
    int i = 0;
    
    while (i < 237)
    {
        int coordinate = rand() % 255;
        if(coordinate != 0 && coordinate != 1 && coordinate != 2 && coordinate != 12 && coordinate != 13 && coordinate != 14
        && coordinate != 15 && coordinate != 16 && coordinate != 17 && coordinate != 27 && coordinate != 28 && coordinate != 29
        && coordinate != 30 && coordinate != 31 && coordinate != 32 && coordinate != 42 && coordinate != 43 && coordinate != 44)
        {
            if(!isIntAlreadyInsideVector(coords, coordinate))
            {
                coords.push_back(coordinate);
                i++;
            }
        }
    }
    
    std::vector<int> xCoords;
    std::vector<int> yCoords;
    for (std::vector<int> ::iterator itr = coords.begin(); itr != coords.end(); itr++)
    {
        int xValue = (*itr);
        int yValue = 0;
        
        while(xValue >= 15)
        {
            xValue -= 15;
            yValue++;
        }
        
        xCoords.push_back(xValue);
        yCoords.push_back(yValue);
    }
    
    std::cout << "\nX Values:" << std::endl;
    for (std::vector<int> ::iterator itr = xCoords.begin(); itr != xCoords.end(); itr++)
    {
        std::cout << "" << (*itr) << ",";
    }
    
    std::cout << "-1" << std::endl;
    
    std::cout << "\nY Values:" << std::endl;
    for (std::vector<int> ::iterator itr = yCoords.begin(); itr != yCoords.end(); itr++)
    {
        std::cout << "" << (*itr) << ",";
    }
    
    std::cout << "-1" << std::endl;
    
    return 0;
}
