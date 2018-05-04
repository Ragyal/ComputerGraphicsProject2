#ifndef MATRIX_H
#define MATRIX_H

#include <vector>


template<class T>
class Matrix
{
public:
	Matrix(unsigned int m, unsigned int n);
	~Matrix();

    unsigned int getM() const { return m; }
    unsigned int getN() const { return n; }

	T getAt(unsigned int i, unsigned int j) const { return this->matrix->at(i)->at(j); }
	void setAt(unsigned int i, unsigned int j, const T value);

    void Print();

private:
    unsigned int m;
    unsigned int n;

	std::vector<std::vector<T>*>* matrix;
};

#endif // MATRIX_H
