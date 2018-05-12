#include "surface.h"

#include <QOpenGLFunctions>
#include <iostream>
#include <fstream>


Surface::Surface(std::string fileName, unsigned int resolution)
{
	this->vertices = new std::vector<Vertex*>();
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

	if (this->degreeM == this->degreeN)
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

	for (unsigned long i = 0; i < this->vertices->size(); i++)
    {
		delete this->vertices->at(i);
    }
	delete this->vertices;
}

void Surface::Print(bool printBezierPoints) const
{

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
			posA = this->vertices->at(bezierPoints->getAt(i, j));
			posB = this->vertices->at(bezierPoints->getAt(i+1, j));
			posC = this->vertices->at(bezierPoints->getAt(i+1, j+1));
			posD = this->vertices->at(bezierPoints->getAt(i, j+1));

			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			glColor3f(1.0, 0.0, 1.0);

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
	std::ifstream file(fileName.c_str());
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
					this->bezierPoints = new Matrix<unsigned int>(m, n);
					created = true;
				}
				else
				{
					std::cout << "file content obstructed" << std::endl;
					return false;
				}
			}
			file >> x >> y >> z;

			i = this->vertices->size();
            pos = new Vertex(x, y, z, i);

			this->vertices->push_back(pos);

			this->bezierPoints->setAt(i/n, i%n, i);
		}
		else if (key == "m")
		{
			file >> m;
			this->degreeM = m - 1;
		}
		else if (key == "n")
		{
			file >> n;
			this->degreeN = n - 1;
		}
		file >> key;
	}
	file.close();
	return true;
}

void Surface::precalcBersteinPolynomials()
{
	this->bersteinSamplesM = new BernsteinPolynomial(this->degreeM, this->resolution);

	if (this->degreeM == this->degreeN)
	{
		this->bersteinSamplesN = this->bersteinSamplesM;
	}
	else
	{
		this->bersteinSamplesN = new BernsteinPolynomial(this->degreeN, this->resolution);
	}
}

void Surface::calcSurface()
{
	this->surface = new Matrix<unsigned int>(resolution+1, resolution+1);

    QVector3D pos, Bij;
    Vertex* v;
	double M, N;
	unsigned int index;

	for (unsigned int s = 0; s < this->resolution+1; s++)
	{
		for (unsigned int t = 0; t < this->resolution+1; t++)
		{
            pos = QVector3D();

			for (unsigned int i = 0; i <= this->degreeM; i++)
			{
				for (unsigned int j = 0; j <= this->degreeN; j++)
				{
					Bij = this->vertices->at(this->bezierPoints->getAt(i, j))->Position();
					M = this->bersteinSamplesM->Get(i, s);
					N = this->bersteinSamplesN->Get(j, t);
                    pos += Bij * M * N;
				}
			}

			index = this->vertices->size();
            v = new Vertex(pos.x(), pos.y(), pos.z(), index);
			this->vertices->push_back(v);
			this->surface->setAt(s, t, index);
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
			q = new Quad(this->vertices, this->quads, a, b, c, d, index);
            this->quads->push_back(q);
        }
    }
}
