#include "mesh.h"

#include<iostream>
#include<fstream>
#include<algorithm>

using namespace std;

vector<Vertex*>* Mesh::Vertices = new vector<Vertex*>();
vector<Quad*>* Mesh::Faces = new vector<Quad*>();

Mesh::Mesh(string fileName, int subdivisions)
{
	readFile(fileName);
	connectMesh();

    calcSubdivisionMask(subdivisions);
	PrintAll();
}

Mesh::~Mesh()
{
	for (unsigned long i = 0; i < Faces->size(); i++)
	{
		delete Faces->at(i);
	}
	delete Faces;

	for (unsigned long i = 0; i < Vertices->size(); i++)
	{
		delete Vertices->at(i);
	}
    delete Vertices;
}

void Mesh::PrintAll() {
	cout << "Vertices:" << endl;
	for (unsigned long i = 0; i < Vertices->size(); i++)
	{
		cout << *Vertices->at(i) << endl;
	}

	cout << endl << "Faces:" << endl;
	for (unsigned long i = 0; i < Faces->size(); i++)
	{
		cout << *Faces->at(i) << endl << endl;
	}
}

void Mesh::DrawMesh(bool drawSurface, bool drawWireframe)
{
	for (unsigned long i = 0; i < Faces->size(); i++)
	{
        Faces->at(i)->Draw(drawSurface, drawWireframe);
	}
}

void Mesh::readFile(string fileName)
{
	string fname("D:/Projekte/Qt/ComputerGraphicsProject1/data/" + fileName);			/// Windows OVE
	//string fname("/Users/ove/Documents/Qt/ComputerGraphicsProject1/data/" + fileName);	/// MAC OVE

    ifstream file(fname.c_str());
    if (!file)
    {
        cout << "error opening file" << endl;
        return;
    }

    string key;
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
			i = Vertices->size();
			v = new Vertex(x, y, z, i);
            Vertices->push_back(v);
        }
        else if (key == "f")
		{
			file >> a >> b >> c >> d;
			i = Faces->size();
			q = new Quad((a-1), (b-1), (c-1), (d-1), i);
            Faces->push_back(q);
        }
        file >> key;
    }
	file.close();
}

void Mesh::connectMesh()
{
	for (unsigned long i = 0; i < Faces->size(); i++)
	{
		Faces->at(i)->CalcAllNeighbors();
	}
}

void Mesh::calcSubdivisionMask(unsigned int n)
{
	unsigned long coutOriginalVertices;
	for (unsigned int i = 0; i < n; i++)
	{
		coutOriginalVertices = Vertices->size();

		calcFaceVertices();
		calcEdgeVertices();
		recalcOriginalVertices(coutOriginalVertices);
		divideQuads();
		connectMesh();
	}
}

void Mesh::calcFaceVertices()
{
	for (unsigned long i = 0; i < Faces->size(); i++)
	{
		Faces->at(i)->CalcFaceVertex();
	}
}

void Mesh::calcEdgeVertices()
{
	for (unsigned long i = 0; i < Faces->size(); i++)
	{
		Faces->at(i)->CalcAllEdgeVertices();
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
		currentVertex = Mesh::Vertices->at(i);
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
	vector<int> foundEdgeVertices = vector<int>();

	for (unsigned long i = 0; i < Faces->size(); i++)
	{
		indexInVertex = Faces->at(i)->ContainsVertex(index);
		if (indexInVertex != -1)
		{
			quad = Faces->at(i);
			n++;
			avgFaceVertices += Vertices->at(quad->FaceVertex())->Position();

			if (find(foundEdgeVertices.begin(), foundEdgeVertices.end(), quad->EdgeVertex(indexInVertex)) == foundEdgeVertices.end())
			{
				avgEdgeVertices += Vertices->at(quad->EdgeVertex(indexInVertex))->Position();
				foundEdgeVertices.push_back(quad->EdgeVertex(indexInVertex));
			}

			if (find(foundEdgeVertices.begin(), foundEdgeVertices.end(), quad->EdgeVertex(indexInVertex - 1)) == foundEdgeVertices.end())
			{
				avgEdgeVertices += Vertices->at(quad->EdgeVertex(indexInVertex - 1))->Position();
				foundEdgeVertices.push_back(quad->EdgeVertex(indexInVertex - 1));
			}
		}
	}

	avgFaceVertices /= n;
	avgEdgeVertices /= n;
}

void Mesh::divideQuads()
{
	vector<Quad*>* newFaces = new vector<Quad*>();

	for (unsigned long i = 0; i < Faces->size(); i++)
	{
		Faces->at(i)->Divide(newFaces);
		delete Faces->at(i);
	}

	delete Faces;

	Faces = newFaces;
}
