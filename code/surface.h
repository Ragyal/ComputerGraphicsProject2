#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <QVector3D>

#include <matrix.h>
#include <vertex.h>
#include <quad.h>
#include <bernsteinpolynomial.h>


class Surface
{
public:
	Surface(std::string fileName, unsigned int resolution = 20);
    ~Surface();

	void Print(bool printBezierPoints = true) const;
	void Draw(bool drawSurface = true, bool drawWireframe = true) const;
	void DrawControlMesh() const;

private:
	std::vector<Vertex*>* vertices;
    std::vector<Quad*>* quads;

	unsigned int degreeM;
	unsigned int degreeN;
	unsigned int resolution;
	Matrix<unsigned int>* bezierPoints;		// type int index		m rows and n columns		OR m+1 rows and n+1 columns

	BernsteinPolynomial* bersteinSamplesM;		// type double [0, 1]	m rows resolution+1 columns OR m+1 rows resolution+1 columns
	BernsteinPolynomial* bersteinSamplesN;		// type double [0, 1]	n rows resolution+1 columns OR n+1 rows resolution+1 columns

	Matrix<unsigned int>* surface;			// type int index		resolution+1 rows resolution+1 columns

	bool readFile(std::string fileName);
	void precalcBersteinPolynomials();
	void calcSurface();
    void generateQuads();
};

#endif // SURFACE_H
