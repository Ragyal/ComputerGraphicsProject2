#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <QVector3D>

#include <vectormatrix.h>


class Surface
{
public:
    Surface(int resolution = 20);
    ~Surface();

    static std::vector<QVector3D*>* Points;

    void Print(bool printBezierPoints = true);

private:
    int resolution;
    VectorMatrix* bezierPoints;

    VectorMatrix* bersteinSamples;
    VectorMatrix* surface;

};

#endif // SURFACE_H
