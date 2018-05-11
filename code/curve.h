#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <QVector3D>

#include <vertex.h>
#include <quad.h>
#include <bernsteinpolynomial.h>


class Curve
{
public:
    Curve(std::string fileName, unsigned int resolution);
    ~Curve();

private:
	std::vector<Vertex*>* vertices;
	std::vector<Quad*>* quads;

	unsigned int degree;
    unsigned int resolution;
    std::vector<unsigned int>* bezierPoints;

	BernsteinPolynomial* bersteinSamples;

	bool readFile(std::string fileName);
    void precalcBersteinPolynomials();
    void calcRotationSurface();
};

#endif // CURVE_H
