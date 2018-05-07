#include "surface.h"

#include <QOpenGLFunctions>
#include <iostream>
#include <fstream>
#include <math.h>


std::vector<QVector3D*>* Surface::Points = new std::vector<QVector3D*>();

Surface::Surface(std::string fileName, unsigned int resolution)
{
    this->resolution = resolution;

	readFile(fileName);
	precalcBersteinPolynomials();
	calcSurface();
}

Surface::~Surface()
{
    delete this->bezierPoints;

    if (this->bersteinSamplesM->getM() == this->bersteinSamplesN->getM() && this->bersteinSamplesM->getN() == this->bersteinSamplesN->getN())
    {
        delete this->bersteinSamplesM;
    }
    else
    {
        delete this->bersteinSamplesM;
        delete this->bersteinSamplesN;
    }

	delete this->surface;

    for (unsigned long i = 0; i < Points->size(); i++)
    {
		delete Points->at(i);
    }
    delete Points;
}

void Surface::Print(bool printBezierPoints) const
{
    if (printBezierPoints)
		this->bezierPoints->Print();
}

void Surface::Draw(bool drawSurface, bool drawWireframe) const
{
	unsigned int m = this->surface->getM();
	unsigned int n = this->surface->getN();

	QVector3D *posA, *posB, *posC, *posD;
	QVector3D rVa, rVb, normalV;

	for (unsigned int i = 0; i < m-1; i++)
	{
		for (unsigned int j = 0; j < n-1; j++)
		{
			posA = Points->at(surface->getAt(i, j));
			posB = Points->at(surface->getAt(i+1, j));
			posC = Points->at(surface->getAt(i+1, j+1));
			posD = Points->at(surface->getAt(i, j+1));

			rVa = *posC - *posA;
			rVb = *posD - *posB;
			normalV = QVector3D::normal(rVa, rVb);

			if (drawSurface)
			{
				glBegin(GL_QUADS);

				glNormal3f(normalV.x(), normalV.y(), normalV.z());

				glVertex3f(posA->x(), posA->y(), posA->z());
				glVertex3f(posB->x(), posB->y(), posB->z());
				glVertex3f(posC->x(), posC->y(), posC->z());
				glVertex3f(posD->x(), posD->y(), posD->z());

				glEnd();
			}

			if (drawWireframe)
			{
				glDisable(GL_LIGHTING);
				glBegin(GL_LINES);
				glColor3f(0.0, 0.0, 0.0);

				glVertex3f(posA->x(), posA->y(), posA->z());
				glVertex3f(posB->x(), posB->y(), posB->z());

				glVertex3f(posB->x(), posB->y(), posB->z());
				glVertex3f(posC->x(), posC->y(), posC->z());

				glVertex3f(posC->x(), posC->y(), posC->z());
				glVertex3f(posD->x(), posD->y(), posD->z());

				glVertex3f(posD->x(), posD->y(), posD->z());
				glVertex3f(posA->x(), posA->y(), posA->z());

				glEnd();
				glEnable(GL_LIGHTING);
			}
		}
	}
}

void Surface::DrawControlMesh() const
{
	unsigned int m = this->bezierPoints->getM();
	unsigned int n = this->bezierPoints->getN();

	QVector3D *posA, *posB, *posC, *posD;

	for (unsigned int i = 0; i < m-1; i++)
	{
		for (unsigned int j = 0; j < n-1; j++)
		{
			posA = Points->at(bezierPoints->getAt(i, j));
			posB = Points->at(bezierPoints->getAt(i+1, j));
			posC = Points->at(bezierPoints->getAt(i+1, j+1));
			posD = Points->at(bezierPoints->getAt(i, j+1));

			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);

			glVertex3f(posA->x(), posA->y(), posA->z());
			glVertex3f(posB->x(), posB->y(), posB->z());

			glVertex3f(posB->x(), posB->y(), posB->z());
			glVertex3f(posC->x(), posC->y(), posC->z());

			glVertex3f(posC->x(), posC->y(), posC->z());
			glVertex3f(posD->x(), posD->y(), posD->z());

			glVertex3f(posD->x(), posD->y(), posD->z());
			glVertex3f(posA->x(), posA->y(), posA->z());

			glEnd();
			glEnable(GL_LIGHTING);
		}
	}
}

void Surface::readFile(std::string fileName)
{
    //std::string fname("D:/Projekte/Qt/ComputerGraphicsProject2/data/" + fileName);				/// Windows OVE
    std::string fname("/Users/ove/Documents/Qt/ComputerGraphicsProject2/data/" + fileName);	/// MAC OVE

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
	double stepSize = 1.0f/resolution;

	this->bersteinSamplesM = new Matrix<double>(m, resolution+1);
	double val;

	for (unsigned int i = 0; i < m; i++)
	{
		for (unsigned int t = 0; t < resolution+1; t++)
		{
			val = nChoosek(this->GetDegreeM(), i) * pow(t*stepSize, i) * pow((1.0f - t*stepSize), (this->GetDegreeM()-i));
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
				val = nChoosek(this->GetDegreeN(), j) * pow(t*stepSize, j) * pow((1.0f - t*stepSize), (this->GetDegreeN()-j));
				this->bersteinSamplesN->setAt(j, t, val);
			}
		}
	}
}

void Surface::calcSurface()
{
	this->surface = new Matrix<unsigned int>(resolution+1, resolution+1);

	unsigned int m = this->GetDegreeM();
	unsigned int n = this->GetDegreeN();

	QVector3D *pos, *Bij;
	double M, N;
	unsigned int index;

	for (unsigned int s = 0; s < resolution+1; s++)
	{
		for (unsigned int t = 0; t < resolution+1; t++)
		{
			pos = new QVector3D();

			for (unsigned int i = 0; i <= m; i++)
			{
				for (unsigned int j = 0; j <= n; j++)
				{
					Bij = Points->at(this->bezierPoints->getAt(i, j));
					M = this->bersteinSamplesM->getAt(i, s);
					N = this->bersteinSamplesN->getAt(j, t);
					*pos += *Bij * M * N;
				}
			}

			index = Points->size();
			Points->push_back(pos);
			surface->setAt(s, t, index);
		}
	}
}
