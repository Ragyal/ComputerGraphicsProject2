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
	double stepSize = 2 / PI / this->resolution;
	Vertex *posA, *posB;
	double a, b;

	for (unsigned int i = 0; i < this->resolution; i++)
	{
		posA = this->vertices->at(this->curve->at(i));
		posB = this->vertices->at(this->curve->at(i+1));

		glBegin(GL_QUAD_STRIP);

		glVertex3f(posA->X(), posA->Y(), posA->Z());
		glVertex3f(posB->X(), posB->Y(), posB->Z());

		for (unsigned int r = 1; r < this->resolution-1; r++)
		{
			a =  r    * stepSize;
			b = (r+1) * stepSize;

			glVertex3f(posA->X(), posA->Y() * cos(a) - posA->Z() * sin(a), posA->Y() * sin(a) + posA->Z() * cos(a));
			glVertex3f(posB->X(), posB->Y() * cos(a) - posB->Z() * sin(a), posB->Y() * sin(a) + posB->Z() * cos(a));

			glVertex3f(posA->X(), posA->Y() * cos(b) - posA->Z() * sin(b), posA->Y() * sin(b) + posA->Z() * cos(b));
			glVertex3f(posB->X(), posB->Y() * cos(b) - posB->Z() * sin(b), posB->Y() * sin(b) + posB->Z() * cos(b));
		}

		glVertex3f(posA->X(), posA->Y(), posA->Z());
		glVertex3f(posB->X(), posB->Y(), posB->Z());

		glEnd();
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

}
