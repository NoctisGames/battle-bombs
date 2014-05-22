//
//  BorderBlock.h
//  bomberparty
//
//  Created by Stephen Gowen on 3/9/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomberparty__BorderBlock__
#define __bomberparty__BorderBlock__

#include "GameObject.h"
#include "BorderBlockType.h"

class BorderBlock : public GameObject
{
public:
    BorderBlock(float x, float y, float width, float height, Border_Block_Type type);
    
    Border_Block_Type getBorderBlockType();
    
private:
    Border_Block_Type m_type;
};

#endif /* defined(__bomberparty__BorderBlock__) */
