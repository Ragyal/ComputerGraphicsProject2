#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <vertex.h>
#include <quad.h>


class Mesh
{
public:
    Mesh(std::string fileName, int subdivisions = 0);
    ~Mesh();

    static std::vector<Vertex*>* Vertices;
    static std::vector<Quad*>* Faces;

	void PrintAll();
    void DrawMesh(bool drawSurface = true, bool drawWireframe = false);

private:
	void readFile(std::string fileName);

	void connectMesh();
	vector<Vertex*>* backupVertices();

	void calcSubdivisionMask(unsigned int n = 1);
	void calcFaceVertices();
	void calcEdgeVertices();
	void recalcOriginalVertices(unsigned long coutOriginalVertices);
	void calcSurroundingPoints(int index, QVector3D& avgFaceVertices, QVector3D& avgEdgeVertices);	// needed for recalcOriginalVertices (Vertex-Mask)
	void divideQuads();
};

#endif // MESH_H
