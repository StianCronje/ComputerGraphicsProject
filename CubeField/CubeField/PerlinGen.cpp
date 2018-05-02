
#include "FastNoise.h"
#include "stdio.h"
#include <string.h>
#include<iostream>
#include<fstream>
#include "PerlinGen.h"
#include <math.h>

int xsize = 50;
int ysize = 50;
float** vertices;
FastNoise gen;


void PerlinGen::setMatrixSize(int x, int y)
{
	float** matrix = new float*[x];
	for (int i = 0; i < y; ++i)
		matrix[i] = new float[x];

	vertices = matrix;
	xsize = x;
	ysize = y;
}
void  PerlinGen::perlinSetValues(int seed, int octave)
{
	int seeds = seed;
	gen.SetSeed(seed);
	int octaves = octave;
	gen.SetFractalOctaves(octaves);
	gen.SetNoiseType(FastNoise::WhiteNoise);
}

void  PerlinGen::popVertexArray()
{
	float num;
	for (int r = 0; r < ysize; r++) {
		for (int c = 0; c < xsize; c++) {
			num = gen.GetNoise(r, c);
			num = num * 100;
			num = ceil(num);
			num = fmod(num, 10.0);
			vertices[r][c] = num;
		}
	}
}

float**  PerlinGen::getVertexArray()
{
	return vertices;
}
