///////////////////////////////////////////////////////////
/// Annika Diekmann, Sven Fröhling, Ove von Stackelberg ///
///////////////////////////////////////////////////////////

#include "quad.h"

#include <iostream>
#include <vector>
#include <QOpenGLFunctions>

#include <mesh.h>
#include <vertex.h>


Quad::Quad(){}

Quad::Quad(std::vector<Vertex*>* points, std::vector<Quad*>* quads, int a, int b, int c, int d, int i)
{
    this->verticesReference = points;
    this->quadsReference = quads;

    this->vertices[0] = a;
    this->vertices[1] = b;
    this->vertices[2] = c;
    this->vertices[3] = d;

	this->faceVertex = -1;
	this->index = i;

    this->normalV = QVector3D();

    this->verticesReference->at(a)->IncreaseValence();
    this->verticesReference->at(b)->IncreaseValence();
    this->verticesReference->at(c)->IncreaseValence();
    this->verticesReference->at(d)->IncreaseValence();

	CalcNormalVector();
}

Quad::~Quad()
{
    verticesReference->at(vertices[0])->DecreaseValence();
    verticesReference->at(vertices[1])->DecreaseValence();
    verticesReference->at(vertices[2])->DecreaseValence();
    verticesReference->at(vertices[3])->DecreaseValence();
}

int Quad::ContainsVertex(int index)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (vertices[i] == index)
			return i;
	}
	return -1;
}

void Quad::Draw(bool drawSurface, bool drawWireframe)
{
    if (drawSurface)
    {
        glBegin(GL_QUADS);
        glNormal3f(normalV.x(), normalV.y(), normalV.z());
        for (int i = 0; i < 4; i++)
        {
            glVertex3f(verticesReference->at(vertices[i])->X(), verticesReference->at(vertices[i])->Y(), verticesReference->at(vertices[i])->Z());
        }
        glEnd();
    }

    if (drawWireframe)
    {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 1.0);
        for (int i = 0; i < 4; i++)
        {
            glVertex3f(verticesReference->at(vertices[i])->X(), verticesReference->at(vertices[i])->Y(), verticesReference->at(vertices[i])->Z());
            if (i != 3)
            {
                glVertex3f(verticesReference->at(vertices[i+1])->X(), verticesReference->at(vertices[i+1])->Y(), verticesReference->at(vertices[i+1])->Z());
            }
            else
            {
                glVertex3f(verticesReference->at(vertices[0])->X(), verticesReference->at(vertices[0])->Y(), verticesReference->at(vertices[0])->Z());
            }
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
}

void Quad::CalcNormalVector()
{
    QVector3D rVa = *verticesReference->at(vertices[2]) - *verticesReference->at(vertices[0]);
    QVector3D rVb = *verticesReference->at(vertices[3]) - *verticesReference->at(vertices[1]);
    normalV = QVector3D::normal(rVa, rVb);
}

void Quad::CalcAllNeighbors()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (neighbors[i] == -1) {
			calcNeighborAt(i);
		}
	}
}

bool Quad::SharesEdge(int vertexA, int vertexB, int index)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (neighbors[i] == -1)
		{
			int localA = vertices[i];
			int localB = vertices[(i == 3 ? 0 : i + 1)];

			if ((localA == vertexA && localB == vertexB) || (localA == vertexB && localB == vertexA))
			{
				neighbors[i] = index;
				return true;
			}
		}
	}
	return false;
}

void Quad::CalcFaceVertex()
{
	QVector3D pos = QVector3D();

	for (unsigned int i = 0; i < 4; i++)
	{
        pos += verticesReference->at(vertices[i])->Position();
	}
	pos /= 4;

    int index = verticesReference->size();
	Vertex* v = new Vertex(pos.x(), pos.y(), pos.z(), index);

    verticesReference->push_back(v);
	faceVertex = index;
}

void Quad::CalcAllEdgeVertices()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (edgeVertices[i] == -1) {
			calcEdgeVertexAt(i);
		}
	}
}

void Quad::InsertEdgeVertexAt(int vertexA, int vertexB, int index)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (edgeVertices[i] == -1)
		{
			int localA = vertices[i];
			int localB = vertices[(i == 3 ? 0 : i + 1)];

			if ((localA == vertexA && localB == vertexB) || (localA == vertexB && localB == vertexA))
			{
				edgeVertices[i] = index;
			}
		}
	}
}

void Quad::Divide(std::vector<Quad*>* newFaces)
{
	int index;
	Quad* q;

	for (unsigned int i = 0; i < 4; i++)
	{
		index = newFaces->size();
		q = new Quad(this->verticesReference, newFaces, vertices[i], edgeVertices[i], faceVertex, edgeVertices[i == 0 ? 3 : i - 1], index);

		q->SetNeighbor(1, i == 3 ? index - 3 : index + 1);
		q->SetNeighbor(2, i == 0 ? index + 3 : index - 1);

		newFaces->push_back(q);
	}
}


void Quad::calcNeighborAt(unsigned int index)
{
	int vertexA = vertices[index];
	int vertexB = vertices[(index == 3 ? 0 : index + 1)];
    Quad* q;

    for (unsigned long i = 0; i < this->quadsReference->size(); i++)
	{
        q = this->quadsReference->at(i);
        if (q->Index() != this->index)
		{
            if (q->SharesEdge(vertexA, vertexB, this->index))
			{
                neighbors[index] = q->Index();
			}
		}
	}
}

void Quad::calcEdgeVertexAt(unsigned int index)
{
	if (neighbors[index] == -1)
	{
        std::cerr << "Neighbor of Quad " << this->index << " at edge " << index << " not calculated. Skipping this edgeVertex!" << std::endl;
		return;
	}

    Quad* neighbor = this->quadsReference->at(neighbors[index]);
	int vertexA = vertices[index];
	int vertexB = vertices[(index == 3 ? 0 : index + 1)];

	QVector3D pos = QVector3D();
    pos += verticesReference->at(vertexA)->Position();
    pos += verticesReference->at(vertexB)->Position();
    pos += verticesReference->at(faceVertex)->Position();
    pos += verticesReference->at(neighbor->FaceVertex())->Position();
	pos /= 4;

    int i = verticesReference->size();
	Vertex* v = new Vertex(pos.x(), pos.y(), pos.z(), i);

    verticesReference->push_back(v);
	edgeVertices[index] = i;
	neighbor->InsertEdgeVertexAt(vertexA, vertexB, i);
}


std::ostream& operator<<(std::ostream& Stream, const Quad& q)
{
	if (q.faceVertex != -1)
	{
        Vertex* faceV = q.verticesReference->at(q.faceVertex);

        return Stream << q.Index() << ": (" << q.vA()        << ", " << q.vB()        << ", " << q.vC()        << ", " << q.vD()  << ")" << std::endl
                      << "  ev("            << q.evA()       << ", " << q.evB()       << ", " << q.evC()       << ", " << q.evD() << ")" << std::endl
                      << "  neighbors("     << q.nA()        << ", " << q.nB()        << ", " << q.nC()        << ", " << q.nD()  << ")" << std::endl
                      << "  normal("        << q.normalV.x() << ", " << q.normalV.y() << ", " << q.normalV.z() << ")"  << std::endl
					  << "  faceV("         << faceV->X()    << ", " << faceV->Y()    << ", " << faceV->Z()    << ")";
	}
	else
	{
        return Stream << q.Index() << ": (" << q.vA()        << ", " << q.vB()        << ", " << q.vC()  << ", " << q.vD()  << ")" << std::endl
                      << "  ev("            << q.evA()       << ", " << q.evB()       << ", " << q.evC() << ", " << q.evD() << ")" << std::endl
                      << "  neighbors("     << q.nA()        << ", " << q.nB()        << ", " << q.nC()  << ", " << q.nD()  << ")" << std::endl
					  << "  normal("        << q.normalV.x() << ", " << q.normalV.y() << ", " << q.normalV.z()   << ")";
	}
}
