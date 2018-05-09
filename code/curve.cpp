#include "curve.h"

#include <iostream>
#include <fstream>


Curve::Curve(std::string fileName, unsigned int resolution)
{
    this->resolution = resolution;

    if (readFile(fileName)) {
        precalcBersteinPolynomials();
        calcRotationSurface();
    }
}

Curve::~Curve()
{
    delete this->bezierPoints;
}

bool Curve::readFile(std::string fileName)
{
    //std::string fname("D:/Projekte/Qt/ComputerGraphicsProject2/data/" + fileName);				/// Windows OVE
    std::string fname("/Users/ove/Documents/Qt/ComputerGraphicsProject2/data/" + fileName);	/// MAC OVE

    std::ifstream file(fname.c_str());
    if (!file)
    {
        std::cout << "error opening file" << std::endl;
        return false;
    }

    bool created = false;

    std::string key;
    int n = 0;
    float x, y;
    unsigned long i;
    QVector3D* pos;

    file >> key;
    while (file)
    {
        if (key == "v")
        {
            if (!created)
            {
                if (n > 0)
                {
                    bezierPoints = new std::vector<unsigned int>(n);
                    created = true;
                }
                else
                {
                    std::cout << "file content obstructed" << std::endl;
                    return false;
                }
            }
            file >> x >> y;
            pos = new QVector3D(x, y, 0);
        }
        else if (key == "n")
        {
            file >> n;
        }
        file >> key;
    }
    file.close();
    return true;
}

void Curve::precalcBersteinPolynomials()
{

}

void Curve::calcRotationSurface()
{

}
