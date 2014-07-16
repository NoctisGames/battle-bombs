//
//  Color.h
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomber_party__Color__
#define __bomber_party__Color__

struct Color
{
    float red;
    float green;
    float blue;
    float alpha;

	Color(float redIn, float greenIn, float blueIn, float alphaIn) : red(redIn), green(greenIn), blue(blueIn), alpha(alphaIn)
	{
		// Empty
	}
};

#endif /* defined(__bomber_party__Color__) */
