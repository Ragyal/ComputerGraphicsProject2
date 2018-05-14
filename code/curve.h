///////////////////////////////////////////////////////////
/// Annika Diekmann, Sven Fröhling, Ove von Stackelberg ///
///////////////////////////////////////////////////////////

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

	void Print() const;
	void DrawControlPoints() const;
	void DrawCurve() const;
	void Draw(bool drawSurface = true, bool drawWireframe = false) const;

private:
	std::vector<Vertex*>* vertices;
	std::vector<Quad*>* faces;

	unsigned int degree;
    unsigned int resolution;

    std::vector<unsigned int>* bezierPoints;
	BernsteinPolynomial* bersteinSamples;

	std::vector<unsigned int>* curve;

	bool readFile(std::string fileName);
    void precalcBersteinPolynomials();
	void calcCurve();
    void calcRotationSurface();
};

#endif // CURVE_H
