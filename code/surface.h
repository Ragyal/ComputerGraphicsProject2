#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <QVector3D>

#include <matrix.h>


class Surface
{
public:
	Surface(std::string fileName, unsigned int resolution = 20);
    ~Surface();

    static std::vector<QVector3D*>* Points;

    void Print(bool printBezierPoints = true);
	void Draw(bool drawSurface = true, bool drawWireframe = true);
	void DrawControlMesh();

private:
	unsigned int resolution;
	Matrix<unsigned int>* bezierPoints;		// type int index		m rows and n columns		OR m+1 rows and n+1 columns

	Matrix<double>* bersteinSamplesM;		// type double [0, 1]	m rows resolution+1 columns OR m+1 rows resolution+1 columns
	Matrix<double>* bersteinSamplesN;		// type double [0, 1]	n rows resolution+1 columns OR n+1 rows resolution+1 columns

	Matrix<unsigned int>* surface;			// type int index		resolution+1 rows resolution+1 columns

	void readFile(std::string fileName);
	void precalcBersteinPolynomials();
	void calcSurface();
};

#endif // SURFACE_H
