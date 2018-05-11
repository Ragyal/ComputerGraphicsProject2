#include "curve.h"

#include <iostream>
#include <fstream>


Curve::Curve(std::string fileName, unsigned int resolution)
{
	this->vertices = new std::vector<Vertex*>();
	this->quads = new std::vector<Quad*>();
	this->resolution = resolution;

    if (readFile(fileName)) {
        precalcBersteinPolynomials();
        calcRotationSurface();
    }
}

Curve::~Curve()
{
    delete this->bezierPoints;
	delete this->bersteinSamples;
}

bool Curve::readFile(std::string fileName)
{
	std::ifstream file(fileName.c_str());
    if (!file)
    {
        std::cout << "error opening file" << std::endl;
        return false;
    }

	this->bezierPoints = new std::vector<unsigned int>();

	std::string key;
    float x, y;
	unsigned long i;
	Vertex* pos;

    file >> key;
    while (file)
    {
        if (key == "v")
        {
            file >> x >> y;

			i = this->vertices->size();
			pos = new Vertex(x, y, 0, i);
			this->vertices->push_back(pos);
			this->bezierPoints->push_back(i);
		}
    }	
    file.close();

	this->degree = this->bezierPoints->size() - 1;

    return true;
}

void Curve::precalcBersteinPolynomials()
{
	this->bersteinSamples = new BernsteinPolynomial(this->degree, this->resolution);
}

void Curve::calcRotationSurface()
{

}
