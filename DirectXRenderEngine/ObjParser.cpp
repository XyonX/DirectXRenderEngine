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
	if ((*pBufPos) >= *pBufSize) {
		(*ppBuf) = (float*)realloc((void*)(*ppBuf), ((*pBufSize) + BUFSIZE) * sizeof(float));
		if ((*ppBuf) == NULL)
			return false;
		(*pBufSize) += BUFSIZ;
	}
	return true;
}

//adds a int value to the ppBuf buffer

bool AddFloatBuffer(int value, int** ppBuf, unsigned long* pBufSize, unsigned long* pBufPos)
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
}

CObjparser::~CObjparser()
{
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

	//File vertexe normal
	float* vnbuf = (float*)malloc(BUFSIZE * sizeof(float));
	unsigned long vnbufsize = BUFSIZE;
	unsigned long vnbufpos = 0;

	//File vertexe texture coordinate
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
		//r = GetToken(fp, buf);

		if (!r) break;

		//vertexes
		if (strcmp(buf, "v") == 0)
		{
			GetToken(fp, buf);
			//AddFloatBuffer
		}

	}


	return false;
}

void CObjparser::Clear()
{
}