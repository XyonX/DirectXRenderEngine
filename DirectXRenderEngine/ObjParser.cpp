#include"pch.h"
#include "ObjParser.h"


//Reads a token from the file and places it in buf
bool GetToken(FILE* fp, char* buf)
{
	buf[0] = 0;
	return (fscanf_s(fp, "%s", buf, BUFSIZE) != EOF);
}

//Skips to the end of the line in the file
bool SkipLine(FILE* fp)
{
	char ch;
	do {
		ch = fgetc(fp);
	} while ((ch != 13) && (ch != EOF));
	return (ch != EOF);
}

//adds a float value to the ppBuf buffer

bool AddFloatBuffer(float value, float** ppBuf, unsigned long* pBufSize, unsigned long* pBufPos)
{
	if ((*ppBuf) == NULL)
		return false;

	(*ppBuf)[*pBufPos] = value;
	(*pBufPos)++;

	if ((*pBufPos) >= *pBufSize) 
	{
		(*ppBuf) = (float*)realloc((void*)(*ppBuf), ((*pBufSize) + BUFSIZE) * sizeof(float));
		if ((*ppBuf) == NULL)
			return false;
		(*pBufSize) += BUFSIZ;
	}
	return true;
}

//adds a int value to the ppBuf buffer

bool AddIntBuffer(int value, int** ppBuf, unsigned long* pBufSize, unsigned long* pBufPos)
{
	if ((*ppBuf) == NULL)
		return false;
	(*ppBuf)[*pBufPos] = value;
	(*pBufPos)++;
	if ((*pBufPos) >= *pBufSize) {
		(*ppBuf) = (int*)realloc((void*)(*ppBuf), ((*pBufSize) + BUFSIZE) * sizeof(float));
		if ((*ppBuf) == NULL)
			return false;
		(*pBufSize) += BUFSIZ;
	}
	return true;
}



CObjparser::CObjparser()
{
	m_pVertex = NULL;
	m_nVertexCount = 0;
}

CObjparser::~CObjparser()
{
	Clear();
}

bool CObjparser::LoadFile(char* szFileName)
{

	//Open the file
	FILE* fp = NULL;
	fopen_s(&fp, szFileName, "rb");
	if (fp == NULL)
		return false;

	//variables
	int r, a, b, c;
	char buf[BUFSIZE];

	//File vertexes
	float* vbuf = (float*)malloc(BUFSIZE * sizeof(float));
	unsigned long vbufsize = BUFSIZE;
	unsigned long vbufpos = 0;

	//File vertex normal
	float* vnbuf = (float*)malloc(BUFSIZE * sizeof(float));
	unsigned long vnbufsize = BUFSIZE;
	unsigned long vnbufpos = 0;

	//File vertex texture coordinate
	float* vtbuf = (float*)malloc(BUFSIZE * sizeof(float));
	unsigned long vtbufsize = BUFSIZE;
	unsigned long vtbufpos = 0;

	//File faces
	int* fbuf = (int*)malloc(BUFSIZE * sizeof(int));
	unsigned long fbufsize = BUFSIZE;
	unsigned long fbufpos = 0;

	//check buffer
	if ((vbuf == NULL) || (vnbuf == NULL) || (vtbuf == NULL) || (fbuf == NULL))
	{
		return false;
	}

	while (!feof(fp))
	{
		r = GetToken(fp, buf);

		if (!r) break;

		//vertexes
		if (strcmp(buf, "v") == 0)
		{
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vbuf, &vbufsize, &vbufpos);
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vbuf, &vbufsize, &vbufpos);
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vbuf, &vbufsize, &vbufpos);
			continue;
		}

		//vertex normals

		if (strcmp(buf, "vn") == 0) {
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vnbuf, &vnbufsize, &vnbufpos);
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vnbuf, &vnbufsize, &vnbufpos);
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vnbuf, &vnbufsize, &vnbufpos);
			continue;
		}

		//vertex texture coordinates
		if (strcmp(buf, "vt") == 0) {
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vtbuf, &vtbufsize, &vtbufpos);
			GetToken(fp, buf);
			AddFloatBuffer((float)atof(buf), &vtbuf, &vtbufsize, &vtbufpos);
			continue;
		}

		//vertex faces
		if (strcmp(buf, "f") == 0) {
			for (int i = 0; i < 3; i++)
			{
				GetToken(fp, buf);
				a = b = c = 0;
				if (strstr(buf, "//") == 0)
					scanf_s(buf, "%d/%d/%d", &a, &b, &c, BUFSIZE);
				else
					scanf_s(buf, "%d//%d", &a, &b, &c, BUFSIZE);
				AddIntBuffer(a, &fbuf, &fbufsize, &fbufpos);
				AddIntBuffer(b, &fbuf, &fbufsize, &fbufpos);
				AddIntBuffer(c, &fbuf, &fbufsize, &fbufpos);
			}
			continue;
		}
		//skip comments and unknown items
		SkipLine(fp);

	}

	//convert (non-indexed)
	m_nVertexCount = fbufpos / 3;
	m_pVertex = (VertexObj*)malloc(m_nVertexCount * sizeof(VertexObj));
	if (m_pVertex == NULL)
		return false;
	::memset((void*)m_pVertex, 0, m_nVertexCount * sizeof(VertexObj));
	unsigned long vi, vti, vni, f;
	for (int i = 0; i < (int)m_nVertexCount; i++)
	{
		f = i * 3;
		vi =  fbuf[f + 0] - 1;
		vti = fbuf[f + 1] - 1;
		vni = fbuf[f + 2] - 1;

		//vertex
		if (vi < vbufpos)
		{
			m_pVertex[i].pos.x = vbuf[vi * 3 + 0];
			m_pVertex[i].pos.y = vbuf[vi * 3 + 1];
			m_pVertex[i].pos.z = vbuf[vi * 3 + 2];
		}
		//texture coordinate
		if (vi < vtbufpos)
		{
			m_pVertex[i].tex0.x = vtbuf[vi * 2 + 0];
			m_pVertex[i].tex0.y = vtbuf[vi * 2 + 1];
		}
		//normal
		if (vni < vnbufpos)
		{
			m_pVertex[i].norm.x = vnbuf[vni * 3 + 0];
			m_pVertex[i].norm.y = vnbuf[vni * 3 + 1];
			m_pVertex[i].norm.z = vnbuf[vni * 3 + 2];
		}
	}
	//cleanup
	free(vbuf);
	free(vnbuf);
	free(vtbuf);
	free(fbuf);
	fclose(fp);


	return true;
}

void CObjparser::Clear()
{
	if (m_pVertex != NULL)
	{
		free(m_pVertex);
	}
	m_pVertex = NULL;
	m_nVertexCount = 0;
}