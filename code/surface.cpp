#include "surface.h"

#include <QOpenGLFunctions>
#include <iostream>
#include <fstream>
#include <math.h>


Surface::Surface(std::string fileName, unsigned int resolution)
{
    this->points = new std::vector<Vertex*>();
    this->quads = new std::vector<Quad*>();
    this->resolution = resolution;

	if (readFile(fileName)) {
		precalcBersteinPolynomials();
		calcSurface();
        generateQuads();
	}
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

    for (unsigned long i = 0; i < this->quads->size(); i++)
    {
        delete this->quads->at(i);
    }
    delete this->quads;

    for (unsigned long i = 0; i < this->points->size(); i++)
    {
        delete this->points->at(i);
    }
    delete this->points;
}

void Surface::Print(bool printBezierPoints) const
{
    if (printBezierPoints)
		this->bezierPoints->Print();
}

void Surface::Draw(bool drawSurface, bool drawWireframe) const
{
    for (unsigned long i = 0; i < this->quads->size(); i++)
    {
        this->quads->at(i)->Draw(drawSurface, drawWireframe);
    }
}

void Surface::DrawControlMesh() const
{
	unsigned int m = this->bezierPoints->getM();
	unsigned int n = this->bezierPoints->getN();

    Vertex *posA, *posB, *posC, *posD;

	for (unsigned int i = 0; i < m-1; i++)
	{
		for (unsigned int j = 0; j < n-1; j++)
		{
            posA = this->points->at(bezierPoints->getAt(i, j));
            posB = this->points->at(bezierPoints->getAt(i+1, j));
            posC = this->points->at(bezierPoints->getAt(i+1, j+1));
            posD = this->points->at(bezierPoints->getAt(i, j+1));

			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);

            glVertex3f(posA->X(), posA->Y(), posA->Z());
            glVertex3f(posB->X(), posB->Y(), posB->Z());

            glVertex3f(posB->X(), posB->Y(), posB->Z());
            glVertex3f(posC->X(), posC->Y(), posC->Z());

            glVertex3f(posC->X(), posC->Y(), posC->Z());
            glVertex3f(posD->X(), posD->Y(), posD->Z());

            glVertex3f(posD->X(), posD->Y(), posD->Z());
            glVertex3f(posA->X(), posA->Y(), posA->Z());

			glEnd();
			glEnable(GL_LIGHTING);
		}
	}
}

bool Surface::readFile(std::string fileName)
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
	int m = 0, n = 0;
	float x, y, z;
	unsigned long i;
    Vertex* pos;

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
					return false;
				}
			}
			file >> x >> y >> z;

            i = this->points->size();
            pos = new Vertex(x, y, z, i);

            this->points->push_back(pos);

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
	return true;
}

/// Source: https://ideone.com/aDJXNO
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

    QVector3D pos, Bij;
    Vertex* v;
	double M, N;
	unsigned int index;

	for (unsigned int s = 0; s < resolution+1; s++)
	{
		for (unsigned int t = 0; t < resolution+1; t++)
		{
            pos = QVector3D();

			for (unsigned int i = 0; i <= m; i++)
			{
				for (unsigned int j = 0; j <= n; j++)
				{
                    Bij = this->points->at(this->bezierPoints->getAt(i, j))->Position();
					M = this->bersteinSamplesM->getAt(i, s);
					N = this->bersteinSamplesN->getAt(j, t);
                    pos += Bij * M * N;
				}
			}

            index = this->points->size();
            v = new Vertex(pos.x(), pos.y(), pos.z(), index);
            this->points->push_back(v);
			surface->setAt(s, t, index);
		}
    }
}

void Surface::generateQuads()
{
    unsigned int m = this->surface->getM();
    unsigned int n = this->surface->getN();

    unsigned long a, b, c, d, index;
    Quad* q;

    for (unsigned int i = 0; i < m-1; i++)
    {
        for (unsigned int j = 0; j < n-1; j++)
        {
            a = surface->getAt(i, j);
            b = surface->getAt(i+1, j);
            c = surface->getAt(i+1, j+1);
            d = surface->getAt(i, j+1);

            index = this->quads->size();
            q = new Quad(this->points, this->quads, a, b, c, d, index);
            this->quads->push_back(q);
        }
    }
}
