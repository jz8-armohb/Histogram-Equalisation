#include <iostream>
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[])
{
	FILE* oriImgPtr;
	FILE* equImgPtr;
	const char* oriImgName = argv[1];
	const char* equImgName = argv[2];
	double greyProb[256] = { 0 };
	double greyCumProb[256] = { 0 };

	/* Open the files */
	if (fopen_s(&oriImgPtr, oriImgName, "rb") == 0)
	{
		cout << "Successfully opened \"" << oriImgName << "\"." << endl;
	}
	else
	{
		cout << "Failed to open \"" << oriImgName << "\"." << endl;
		exit(-1);
	}
	if (fopen_s(&equImgPtr, equImgName, "wb") == 0)
	{
		cout << "Successfully opened \"" << equImgName << "\"." << endl;
	}
	else
	{
		cout << "Failed to open \"" << equImgName << "\"." << endl;
		exit(-1);
	}

	/* Space allocation */
	unsigned char* oriYBuff = new unsigned char[w * h];
	unsigned char* equYBuff = new unsigned char[w * h];
	unsigned char* equUBuff = new unsigned char[w * h / 4];
	unsigned char* equVBuff = new unsigned char[w * h / 4];
	int* greyFreq = new int[256];

	/* Initialisation of U & V component (greyscale image) */
	memset(equUBuff, 128, w * h / 4);
	memset(equVBuff, 128, w * h / 4);


	/* Read Y component into the buffer */
	fread(oriYBuff, sizeof(unsigned char), w * h, oriImgPtr);

	/* Calculate probabilities of each grey value */
	Freq(oriYBuff, greyProb);

	/* Calculate cumulative probabilites of each grey value */
	CumulativeFreq(greyProb, greyCumProb);

	/* Mapping */
	Mapping(greyCumProb, oriYBuff, equYBuff);

	/* Write histogram-equalised data into the new file */
	fwrite(equYBuff, sizeof(unsigned char), w * h, equImgPtr);
	fwrite(equUBuff, sizeof(unsigned char), w * h / 4, equImgPtr);
	fwrite(equVBuff, sizeof(unsigned char), w * h / 4, equImgPtr);

	delete[]oriYBuff;
	delete[]equYBuff;
	delete[]equUBuff;
	delete[]equVBuff;
	fclose(oriImgPtr);
	fclose(equImgPtr);
}


void Freq(unsigned char* yBuff, double prob[])
{
	double count[256] = { 0 };
	for (int i = 0; i < w * h; i++)
	{
		int greyIndex = (int)yBuff[i];
		count[greyIndex]++;
	}

	for (int i = 0; i < 256; i++)
	{
		prob[i] = count[i] / (w * h);
		//printf("%-5d%lf\n", i, prob[i]);
	}
}


void CumulativeFreq(double prob[], double cumProb[])
{
	cumProb[0] = 0;
	//printf("%-5d%lf\n", 0, cumProb[0]);

	for (int i = 1; i < 256; i++)
	{
		cumProb[i] = cumProb[i - 1] + prob[i - 1];
		//printf("%-5d%lf\n", i, cumProb[i]);
	}
}


void Mapping(double cumProb[], unsigned char* yBuffOri, unsigned char* yBuffEqu)
{
	for (int i = 0; i < 256; i++)
	{
		cumProb[i] = floor(255 * cumProb[i] + 0.5);
	}

	for (int i = 0; i < w * h; i++)
	{
		int greyIndex = (int)yBuffOri[i];
		yBuffEqu[i] = cumProb[greyIndex];
	}
}