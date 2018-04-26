#pragma once
#ifndef PERLINGEN_H
#define PERLINGEN_H

class PerlinGen {
	int seeds;
	int octaves;
public:

	void setMatrixSize(int);
	void perlinSetValues(int, int);
	void popVertexArray();
	float** getVertexArray();
};
#endif