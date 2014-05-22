//
//  macros.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef Macros_H
#define Macros_H

#ifdef TECHNE_GAMES_SERVER_WINDOWS
#define M_PI		3.14159265358979323846
#endif

#define UNUSED(x) (void)(x)

#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0f * M_PI)
#define RADIANS_TO_DEGREES(angle) ((angle) / M_PI * 180.0f)

#ifdef TECHNE_GAMES_DIRECT_3D
#define DEGREES_TO_RADIANS_WP(angle) -((angle) / 180.0f * M_PI)
#endif

#endif /* Macros_H */