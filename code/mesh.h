///////////////////////////////////////////////////////////
/// Annika Diekmann, Sven Fröhling, Ove von Stackelberg ///
///////////////////////////////////////////////////////////

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

	void PrintAll();
    void DrawMesh(bool drawSurface = true, bool drawWireframe = false);

private:
    std::vector<Vertex*>* vertices;
    std::vector<Quad*>* faces;

	bool readFile(std::string fileName);

	void connectMesh();
    std::vector<Vertex*>* backupVertices();

	void calcSubdivisionMask(unsigned int n = 1);
	void calcFaceVertices();
	void calcEdgeVertices();
	void recalcOriginalVertices(unsigned long countOriginalVertices);
	void calcSurroundingPoints(int index, QVector3D& avgFaceVertices, QVector3D& avgEdgeVertices);	// needed for recalcOriginalVertices (Vertex-Mask)
	void divideQuads();
};

#endif // MESH_H
