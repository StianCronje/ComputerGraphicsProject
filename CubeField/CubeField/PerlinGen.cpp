
#include "FastNoise.h"
#include "stdio.h"
#include <string.h>
#include<iostream>
#include<fstream>

using namespace std;

float vertices[50][50];
FastNoise gen;
class PerlinGen {
	int seeds;
	int octaves;
public:
	
	void perlinSetValues(int, int);
	void popVertexArray();
	void getVertexArray(float[50][50]);
};

void PerlinGen::perlinSetValues(int seed, int octave)
{
	seeds = seed;
	octaves = octave;
	gen.SetNoiseType(FastNoise::PerlinFractal);
}

void PerlinGen::popVertexArray()
{
	float num;
	for (int r = 0; r < 50; r++) {
		for (int c = 0; c < 50; c++) {
			num = gen.GetNoise(r, c);
			num = num * 100;
			num = ceil(num);
			num = fmod(num, 10.0);
			vertices[r][c] = num;
		}
	}
}

void PerlinGen::getVertexArray(float array[50][50])
{
	for(int h = 0; h < 50; h++)
	{
		for (int w = 0; w < 50; w++)
		{

			array[h][w] = vertices[h][w];
		}
	}
}
/* Test
int main()
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
