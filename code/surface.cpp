#include "surface.h"


std::vector<QVector3D*>* Surface::Points = new std::vector<QVector3D*>();

Surface::Surface(int resolution)
{
    this->resolution = resolution;
    this->bezierPoints = new VectorMatrix(3, 2);
}

Surface::~Surface()
{
    delete this->bezierPoints;

    for (unsigned long i = 0; i < Points->size(); i++)
    {
        delete Points->at(i);
    }
    delete Points;
}

void Surface::Print(bool printBezierPoints)
{
    if (printBezierPoints)
        this->bezierPoints->Print();
}
