#pragma once
extern int w;
extern int h;

void Freq(unsigned char* yBuff, double freq[]);
void CumulativeFreq(double prob[], double cumProb[]);
void Mapping(double cumProb[], unsigned char* yBuffOri, unsigned char* yBuffEqu);