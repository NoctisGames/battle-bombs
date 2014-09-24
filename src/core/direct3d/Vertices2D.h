//
//  Vertices2D.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Vertices2D__
#define __battle_bombs__Vertices2D__

struct BASIC_VERTEX
{
	float X, Y, Z; // vertex position
};

struct COLOR_VERTEX
{
	float X, Y, Z; // vertex position
	float R, G, B, A; // vertex color
};

struct TEXTURE_VERTEX
{
	float X, Y, Z; // vertex position
	float R, G, B, A; // vertex color
	float U, V;    // texture coordinates
};

#include <d3d11_1.h>
#include <memory>
#include <vector>

using namespace Microsoft::WRL;

class Vertices2D
{
public:
	Vertices2D(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, int maxNumVertices, bool isUsingTextureCoordinates, bool isUsingColors);
    
    void resetIndex();
    
	void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v);
    
	void bind();

private:
	ID3D11Device1 *dev;                      // the device interface
	ID3D11DeviceContext1 *devcon;            // the device context interface
	ComPtr<ID3D11VertexShader> vertexshader;        // the vertex shader interface
	ComPtr<ID3D11PixelShader> pixelshader;          // the pixel shader interface
	ComPtr<ID3D11InputLayout> inputlayout;          // the input layout interface
	ComPtr<ID3D11Buffer> constantbuffer;            // the constant buffer interface
	ComPtr<ID3D11Buffer> vertexbuffer;              // the vertex buffer interface
	std::vector<BASIC_VERTEX> m_basicVertices;
	std::vector<COLOR_VERTEX> m_colorVertices;
	std::vector<TEXTURE_VERTEX> m_textureVertices;
    int m_iVerticesIndex;
	int m_iMaxNumVertices;
    bool m_hasColor;
    bool m_hasTextureCoordinates;
};

#endif /* defined(__battle_bombs__Vertices2D__) */