//
//  platform_macros.h
//  battle-bombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef Platform_Macros_H
#define Platform_Macros_H

#if defined(__GNUC__)
#define PRINTF_ATTRIBUTE(format_pos, arg_pos) __attribute__((format(printf, format_pos, arg_pos)))
#else
#define PRINTF_ATTRIBUTE(format_pos, arg_pos)
#endif

#endif /* Platform_Macros_H */