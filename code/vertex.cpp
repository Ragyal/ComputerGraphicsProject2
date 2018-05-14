///////////////////////////////////////////////////////////
/// Annika Diekmann, Sven Fröhling, Ove von Stackelberg ///
///////////////////////////////////////////////////////////

#include "vertex.h"


Vertex::Vertex(float x, float y, float z, int i)
{
    this->position = QVector3D(x, y, z);
	this->index = i;
	this->valence = 0;
}

Vertex::Vertex(const Vertex* v)
{
	this->position = QVector3D(v->X(), v->Y(), v->Z());
	this->index = v->Index();
	this->valence = v->Valence();
}

QVector3D operator-(const Vertex& vA, const Vertex& vB)
{
	return vA.position - vB.position;
}

std::ostream& operator<<(std::ostream& Stream, const Vertex& v)
{
	return Stream << v.Index() << ": (" << v.X() << ", " << v.Y() << ", " << v.Z() << "), valence = " << v.Valence();
}
