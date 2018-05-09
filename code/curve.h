#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <QVector3D>


class Curve
{
public:
    Curve(std::string fileName, unsigned int resolution);
    ~Curve();

private:
    unsigned int resolution;
    std::vector<unsigned int>* bezierPoints;

    bool readFile(std::string fileName);
    void precalcBersteinPolynomials();
    void calcRotationSurface();
};

#endif // CURVE_H
