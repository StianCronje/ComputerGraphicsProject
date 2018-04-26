
#include "FastNoise.h"
#include "stdio.h"
#include <string.h>
#include<iostream>
#include<fstream>
#include "PerlinGen.h"

int size;
float** vertices;
FastNoise gen;


void PerlinGen::setMatrixSize(int s)
{
	float** matrix = new float*[s];
	for (int i = 0; i < s; ++i)
		matrix[i] = new float[s];

	vertices = matrix;
	size = s;
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
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
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
	/*for(int h = 0; h < size; h++)
	{
	for (int w = 0; w < size; w++)
	{

	array[h][w] = vertices[h][w];
	}
	}*/
	return vertices;
}

/*int main() Test
{
PerlinGen gener;
gener.perlinSetValues(100, 5);
gener.popVertexArray();
float vertex[50][50];
gener.getVertexArray(vertex);

for (int h = 0; h < 50; h++)
{
for (int w = 0; w < 50; w++)
{

cout << vertex[h][w] << " ";
}
cout << endl;
}
}*/
