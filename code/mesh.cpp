#include "mesh.h"

#include<iostream>
#include<fstream>
#include<algorithm>


Mesh::Mesh(std::string fileName, int subdivisions)
{
    this->vertices = new std::vector<Vertex*>();
    this->faces = new std::vector<Quad*>();

	readFile(fileName);
	connectMesh();

    calcSubdivisionMask(subdivisions);
	PrintAll();
}

Mesh::~Mesh()
{
    for (unsigned long i = 0; i < faces->size(); i++)
	{
        delete faces->at(i);
	}
    delete faces;

    for (unsigned long i = 0; i < vertices->size(); i++)
	{
        delete vertices->at(i);
	}
    delete vertices;
}

void Mesh::PrintAll() {
    std::cout << "Vertices:" << std::endl;
    for (unsigned long i = 0; i < this->vertices->size(); i++)
	{
        std::cout << *this->vertices->at(i) << std::endl;
	}

    std::cout << std::endl << "Faces:" << std::endl;
    for (unsigned long i = 0; i < this->faces->size(); i++)
	{
        std::cout << *this->faces->at(i) << std::endl << std::endl;
	}
}

void Mesh::DrawMesh(bool drawSurface, bool drawWireframe)
{
    for (unsigned long i = 0; i < this->faces->size(); i++)
	{
        this->faces->at(i)->Draw(drawSurface, drawWireframe);
	}
}

void Mesh::readFile(std::string fileName)
{
    std::string fname("D:/Projekte/Qt/ComputerGraphicsProject1/data/" + fileName);			/// Windows OVE
    //std::string fname("/Users/ove/Documents/Qt/ComputerGraphicsProject1/data/" + fileName);	/// MAC OVE

    std::ifstream file(fname.c_str());
    if (!file)
    {
        std::cout << "error opening file" << std::endl;
        return;
    }

    std::string key;
    float x, y, z;
	int a, b, c, d, i;
    Vertex* v;
    Quad* q;

	file >> key;
    while (file)
	{
		if (key == "v")
        {
            file >> x >> y >> z;
            i = this->vertices->size();
			v = new Vertex(x, y, z, i);
            this->vertices->push_back(v);
        }
        else if (key == "f")
		{
			file >> a >> b >> c >> d;
            i = this->faces->size();
            q = new Quad(this->vertices, this->faces, (a-1), (b-1), (c-1), (d-1), i);
            this->faces->push_back(q);
        }
        file >> key;
    }
	file.close();
}

void Mesh::connectMesh()
{
    for (unsigned long i = 0; i < this->faces->size(); i++)
	{
        this->faces->at(i)->CalcAllNeighbors();
	}
}

void Mesh::calcSubdivisionMask(unsigned int n)
{
	unsigned long coutOriginalVertices;
	for (unsigned int i = 0; i < n; i++)
	{
        coutOriginalVertices = this->vertices->size();

		calcFaceVertices();
		calcEdgeVertices();
		recalcOriginalVertices(coutOriginalVertices);
		divideQuads();
		connectMesh();
	}
}

void Mesh::calcFaceVertices()
{
    for (unsigned long i = 0; i < this->faces->size(); i++)
	{
        this->faces->at(i)->CalcFaceVertex();
	}
}

void Mesh::calcEdgeVertices()
{
    for (unsigned long i = 0; i < this->faces->size(); i++)
	{
        this->faces->at(i)->CalcAllEdgeVertices();
	}
}

void Mesh::recalcOriginalVertices(unsigned long coutOriginalVertices)
{
	Vertex* currentVertex;
	int n;
	QVector3D currentPos;
	QVector3D avgFaceVertices;
	QVector3D avgEdgeVertices;
	QVector3D newPos;

	for (unsigned long i = 0; i < coutOriginalVertices; i++)
	{
        currentVertex = this->vertices->at(i);
		currentPos = currentVertex->Position();
		n = currentVertex->Valence();
		avgFaceVertices = QVector3D();
		avgEdgeVertices = QVector3D();

		calcSurroundingPoints(i, avgFaceVertices, avgEdgeVertices);

		newPos = 1.0f/n * (4*avgEdgeVertices - avgFaceVertices + currentPos * (n - 3));
		currentVertex->Position(newPos);
	}
}

void Mesh::calcSurroundingPoints(int index, QVector3D& avgFaceVertices, QVector3D& avgEdgeVertices)
{
	int n = 0;
	int indexInVertex;
	Quad* quad;
    std::vector<int> foundEdgeVertices = std::vector<int>();

    for (unsigned long i = 0; i < this->faces->size(); i++)
	{
        indexInVertex = this->faces->at(i)->ContainsVertex(index);
		if (indexInVertex != -1)
		{
            quad = this->faces->at(i);
			n++;
            avgFaceVertices += this->vertices->at(quad->FaceVertex())->Position();

			if (find(foundEdgeVertices.begin(), foundEdgeVertices.end(), quad->EdgeVertex(indexInVertex)) == foundEdgeVertices.end())
			{
                avgEdgeVertices += this->vertices->at(quad->EdgeVertex(indexInVertex))->Position();
				foundEdgeVertices.push_back(quad->EdgeVertex(indexInVertex));
			}

			if (find(foundEdgeVertices.begin(), foundEdgeVertices.end(), quad->EdgeVertex(indexInVertex - 1)) == foundEdgeVertices.end())
			{
                avgEdgeVertices += this->vertices->at(quad->EdgeVertex(indexInVertex - 1))->Position();
				foundEdgeVertices.push_back(quad->EdgeVertex(indexInVertex - 1));
			}
		}
	}

	avgFaceVertices /= n;
	avgEdgeVertices /= n;
}

void Mesh::divideQuads()
{
    std::vector<Quad*>* newFaces = new std::vector<Quad*>();

    for (unsigned long i = 0; i < this->faces->size(); i++)
	{
        this->faces->at(i)->Divide(newFaces);
        delete this->faces->at(i);
	}

    delete this->faces;

    this->faces = newFaces;
}
