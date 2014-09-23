//
//  Direct3DRectangleRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__Direct3DRectangleRenderer__
#define __battlebombs__Direct3DRectangleRenderer__

#include <d3d11_1.h>
#include <memory>
#include "Color.h"

class Vertices2D;
class Rectangle;

using namespace Microsoft::WRL;

class Direct3DRectangleRenderer
{
public:
	Direct3DRectangleRenderer(ComPtr<ID3D11Device1> d3dDevice, ComPtr<ID3D11DeviceContext1> d3dContext, bool useColor, bool isFill);
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
	void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
    
private:
	ComPtr<ID3D11Device1> dev;                      // the device interface
	ComPtr<ID3D11DeviceContext1> devcon;            // the device context interface
	ComPtr<ID3D11BlendState> blendstate;            // the blend state interface
	ComPtr<ID3D11Buffer> indexbuffer;               // the index buffer interface
    std::unique_ptr<Vertices2D> m_vertices;
	std::unique_ptr<short> m_indices;
	int m_iNumRectangles;
    bool m_useColor;
    bool m_isFill;

	void generateIndices(int maxSprites);
};

#endif /* defined(__battlebombs__Direct3DRectangleRenderer__) */
