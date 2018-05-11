#ifndef QUAD_H
#define QUAD_H

#include <vector>
#include <QVector3D>

#include <vertex.h>


class Quad
{
public:
    Quad();
    Quad(std::vector<Vertex*>* points, std::vector<Quad*>* quads, int a, int b, int c, int d, int i );
    ~Quad();

	int Index() const {return index;}

	int GetVertex(int i) const {return vertices[i < 0 ? 3 : i];}
	int vA() const {return vertices[0];}
	int vB() const {return vertices[1];}
	int vC() const {return vertices[2];}
	int vD() const {return vertices[3];}

	int FaceVertex() const {return faceVertex;}

	int EdgeVertex(int i) const {return edgeVertices[i < 0 ? 3 : i];}
	int evA() const {return edgeVertices[0];}
	int evB() const {return edgeVertices[1];}
	int evC() const {return edgeVertices[2];}
	int evD() const {return edgeVertices[3];}

	int GetNeighbor(int i) const {return neighbors[i < 0 ? 3 : i];}
	int nA() const {return neighbors[0];}
	int nB() const {return neighbors[1];}
	int nC() const {return neighbors[2];}
	int nD() const {return neighbors[3];}
	void SetNeighbor(int i, int index) {neighbors[i] = index;}

	int ContainsVertex(int index);

    void Draw(bool drawSurface, bool drawWireframe);
	void CalcNormalVector();

	void CalcAllNeighbors();
	bool SharesEdge(int vertexA, int vertexB, int index);			// true if Quad has an edge with vertexA and vertexB, Quad then also inserts index (of calling Quad) at this edge

	void CalcFaceVertex();
	void CalcAllEdgeVertices();
	void InsertEdgeVertexAt(int vertexA, int vertexB, int index);	// inserts index of EdgeVertex on edge with vertexA and vertexB

	void Divide(std::vector<Quad*>* newFaces);								// divides Quad up into four new Quads and adds them to a new Quad vector

    friend std::ostream& operator<<(std::ostream& Stream, const Quad& q);

private:
    std::vector<Vertex*>* verticesReference;
    std::vector<Quad*>* quadsReference;
	int index;
    int vertices[4] = {-1, -1, -1, -1};
	int faceVertex;
	int edgeVertices[4] = {-1, -1, -1, -1};
	int neighbors[4] = {-1, -1, -1, -1};
    QVector3D normalV;

	void calcNeighborAt(unsigned int index);
	void calcEdgeVertexAt(unsigned int index);
};

#endif // QUAD_H
