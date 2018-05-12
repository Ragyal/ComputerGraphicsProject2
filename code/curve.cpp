#include "curve.h"

#include <QOpenGLFunctions>
#include <iostream>
#include <fstream>
#include <math.h>

#define PI 3.14159265


Curve::Curve(std::string fileName, unsigned int resolution)
{
	this->vertices = new std::vector<Vertex*>();
	this->quads = new std::vector<Quad*>();
	this->resolution = resolution;

    if (readFile(fileName)) {
        precalcBersteinPolynomials();
		calcCurve();
        calcRotationSurface();
    }
}

Curve::~Curve()
{
    delete this->bezierPoints;
	delete this->bersteinSamples;
}

void Curve::DrawControlPoints() const
{
	Vertex *posA, *posB;

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);

	for (unsigned long i = 0; i < this->bezierPoints->size()-1; i++)
	{
		posA = this->vertices->at(this->bezierPoints->at(i));
		posB = this->vertices->at(this->bezierPoints->at(i+1));

		glVertex3f(posA->X(), posA->Y(), posA->Z());
		glVertex3f(posB->X(), posB->Y(), posB->Z());
	}

	glEnd();
	glEnable(GL_LIGHTING);
}

void Curve::DrawCurve() const
{
	Vertex *posA, *posB;

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);

	for (unsigned long i = 0; i < this->curve->size()-1; i++)
	{
		posA = this->vertices->at(this->curve->at(i));
		posB = this->vertices->at(this->curve->at(i+1));

		glVertex3f(posA->X(), posA->Y(), posA->Z());
		glVertex3f(posB->X(), posB->Y(), posB->Z());
	}

	glEnd();
	glEnable(GL_LIGHTING);
}

void Curve::Draw(bool drawSurface, bool drawWireframe) const
{
	for (unsigned long i = 0; i < this->quads->size(); i++)
	{
		this->quads->at(i)->Draw(drawSurface, drawWireframe);
	}
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
		file >> key;
    }	
    file.close();

	this->degree = this->bezierPoints->size() - 1;

    return true;
}

void Curve::precalcBersteinPolynomials()
{
	this->bersteinSamples = new BernsteinPolynomial(this->degree, this->resolution);
}

void Curve::calcCurve()
{
	this->curve = new std::vector<unsigned int>(resolution+1);

	QVector3D pos, Bi;
	Vertex* v;
	double N;
	unsigned int index;

	for (unsigned int t = 0; t < this->resolution+1; t++)
	{
		pos = QVector3D();

		for (unsigned int i = 0; i <= this->degree; i++)
		{
			Bi = this->vertices->at(this->bezierPoints->at(i))->Position();
			N = this->bersteinSamples->Get(i, t);
			pos += Bi * N;
		}

		index = this->vertices->size();
		v = new Vertex(pos.x(), pos.y(), pos.z(), index);
		this->vertices->push_back(v);
		this->curve->at(t) = index;
	}
}

void Curve::calcRotationSurface()
{
	double stepSize = 360.0 / this->resolution;
	Vertex *posA, *posB;
	double A;
	unsigned int a, b, c, d;
	float newY, newZ;
	unsigned long index;
	std::vector<unsigned int> *verticesA, *verticesB;
	Vertex* v;
	Quad* q;

	for (unsigned int i = 0; i < this->curve->size()-1; i++)
	{
		posA = this->vertices->at(this->curve->at(i));
		posB = this->vertices->at(this->curve->at(i+1));

		if (i == 0)
		{
			verticesA = new std::vector<unsigned int>(this->resolution, this->curve->at(i));

			for (unsigned int r = 1; r < this->resolution; r++)
			{
				A = (r * stepSize) * PI / 180.0;
				newY = posA->Y() * cos(A) - posA->Z() * sin(A);
				newZ = posA->Y() * sin(A) + posA->Z() * cos(A);
				index = this->vertices->size();

				v = new Vertex(posA->X(), newY, newZ, index);
				this->vertices->push_back(v);
				verticesA->at(r) = index;
			}
		}
		else
		{
			verticesA = verticesB;
		}

		verticesB = new std::vector<unsigned int>(this->resolution, this->curve->at(i+1));

		for (unsigned int r = 1; r < this->resolution; r++)
		{
			A = (r * stepSize) * PI / 180.0;
			newY = posB->Y() * cos(A) - posB->Z() * sin(A);
			newZ = posB->Y() * sin(A) + posB->Z() * cos(A);
			index = this->vertices->size();

			v = new Vertex(posB->X(), newY, newZ, index);
			this->vertices->push_back(v);
			verticesB->at(r) = index;
		}

		for (unsigned int r = 0; r < this->resolution; r++)
		{
			a = verticesA->at(r);
			d = verticesB->at(r);

			if (r == this->resolution-1)
			{
				b = verticesA->at(0);
				c = verticesB->at(0);
			}
			else
			{
				b = verticesA->at(r+1);
				c = verticesB->at(r+1);
			}

			index = this->quads->size();
			q = new Quad(this->vertices, this->quads, a, b, c, d, index);
			this->quads->push_back(q);
		}

		delete verticesA;
	}
	delete verticesB;
}
