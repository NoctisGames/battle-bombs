//
//  pch.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef PCH_H
#define PCH_H

#define LOGGING_ON 1

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef TECHNE_GAMES_OPENGL_ES

#elif defined TECHNE_GAMES_DIRECT_3D
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <xaudio2.h>
#include <mmreg.h>
#pragma comment( lib, "xaudio2.lib")
#pragma comment( lib, "mfplat.lib")
#endif

#endif /* PCH_H */
