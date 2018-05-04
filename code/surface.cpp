#include "surface.h"

#include<iostream>
#include<fstream>


std::vector<QVector3D*>* Surface::Points = new std::vector<QVector3D*>();

Surface::Surface(std::string fileName, int resolution)
{
    this->resolution = resolution;

	readFile(fileName);
	precalcBersteinPolynomials();
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

void Surface::readFile(std::string fileName)
{
	std::string fname("D:/Projekte/Qt/ComputerGraphicsProject2/data/" + fileName);				/// Windows OVE
	//string fname("/Users/ove/Documents/Qt/ComputerGraphicsProject2/data/" + fileName);	/// MAC OVE

	std::ifstream file(fname.c_str());
	if (!file)
	{
		std::cout << "error opening file" << std::endl;
		return;
	}

	bool created = false;

	std::string key;
	int m = 0, n = 0;
	float x, y, z;
	unsigned long i;
	QVector3D* pos;

	file >> key;
	while (file)
	{
		if (key == "v")
		{
			if (!created)
			{
				if (m > 0 && n > 0)
				{
					bezierPoints = new Matrix<unsigned int>(m, n);
					created = true;
				}
				else
				{
					std::cout << "file content obstructed" << std::endl;
					return;
				}
			}
			file >> x >> y >> z;
			pos = new QVector3D(x, y, z);

			i = Points->size();
			Points->push_back(pos);

			bezierPoints->setAt(i/n, i%n, i);
		}
		else if (key == "m")
		{
			file >> m;
		}
		else if (key == "n")
		{
			file >> n;
		}
		file >> key;
	}
	file.close();
}

void Surface::precalcBersteinPolynomials()
{

}
