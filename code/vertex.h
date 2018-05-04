#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <QVector3D>

using namespace std;


class Vertex
{
public:
	Vertex(float x = 0, float y = 0, float z = 0, int i = 0);
	Vertex(const Vertex* v);
    ~Vertex(){}

	int Index() const {return index;}

	QVector3D Position() const {return position;}
	float X() const {return position.x();}
	float Y() const {return position.y();}
	float Z() const {return position.z();}
	void Position(QVector3D pos) {position = pos;}

	int Valence() const {return valence;}

	void IncreaseValence() {valence++;}
	void DecreaseValence() {valence--;}

	friend QVector3D operator-(const Vertex& vA, const Vertex& vB);
	friend ostream& operator<<(ostream& Stream, const Vertex& v);


private:
	int index;
	QVector3D position;
    int valence;
};

#endif // VERTEX_H
