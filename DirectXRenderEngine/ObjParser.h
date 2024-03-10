#pragma once
#include <stdio.h>
#include <string>
#include <DirectXMath.h>

using namespace DirectX;

#define BUFSIZE 65536



struct VertexObj
{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
	XMFLOAT3 norm;

};


class CObjparser
{


	//Constructor
public:
	CObjparser();
	~CObjparser();


	//methods
public:
	bool LoadFile(char* szFileName);
	void Clear();


	//Attributes
public:
	VertexObj* m_pVertex;
	unsigned long m_nVertexCount;

};
