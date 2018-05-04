#include "surface.h"

#include <iostream>
#include <fstream>
#include <math.h>


std::vector<QVector3D*>* Surface::Points = new std::vector<QVector3D*>();

Surface::Surface(std::string fileName, unsigned int resolution)
{
    this->resolution = resolution;

	readFile(fileName);
	precalcBersteinPolynomials();
}

Surface::~Surface()
{
    delete this->bezierPoints;
	delete this->bersteinSamplesM;
	delete this->bersteinSamplesN;

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

/// Source: https://ideone.com/aDJXNO   --- NOCH NICHT GETESTET!!!   <-- FALLS FEHLER AUFTRETEN
unsigned int nChoosek(unsigned int n, unsigned int k)
{
	if (k > n)
		return 0;
	if (k * 2 > n)
		k = n-k;
	if (k == 0)
		return 1;

	int result = n;
	for (unsigned int i = 2; i <= k; ++i)
	{
		result *= (n-i+1);
		result /= i;
	}
	return result;
}

void Surface::precalcBersteinPolynomials()
{
	unsigned int m = this->bezierPoints->getM();
	unsigned int n = this->bezierPoints->getN();

	this->bersteinSamplesM = new Matrix<double>(m, resolution+1);
	double val;

	for (unsigned int i = 0; i < m; i++)
	{
		for (unsigned int t = 0; t < resolution+1; t++)
		{
			val = nChoosek(m, i) * pow(t, i) * pow((1.0f - t), (m-i));
			this->bersteinSamplesM->setAt(i, t, val);
		}
	}

	if (m == n)
	{
		this->bersteinSamplesN = this->bersteinSamplesM;
	}
	else
	{
		this->bersteinSamplesN = new Matrix<double>(n, resolution+1);

		for (unsigned int j = 0; j < n; j++)
		{
			for (unsigned int t = 0; t < resolution+1; t++)
			{
				val = nChoosek(n, j) * pow(t, j) * pow((1.0f - t), (n-j));
				this->bersteinSamplesN->setAt(j, t, val);
			}
		}
	}
}
