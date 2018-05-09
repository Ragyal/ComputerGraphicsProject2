#include "vectormatrix.h"

#include <iostream>


VectorMatrix::VectorMatrix(unsigned int m, unsigned int n)
{
    this->m = m;
    this->n = n;


    this->matrix = new std::vector<std::vector<int>*>(m);
    for (unsigned int i = 0; i < m; i++)
	{
		matrix->at(m) = new std::vector<int>(n, -1);
    }
}

VectorMatrix::~VectorMatrix()
{
    for (unsigned int i = 0; i < matrix->size(); i++)
    {
        delete matrix->at(i);
    }
    delete matrix;
}

void VectorMatrix::setAt(unsigned int i, unsigned int j, const int index)
{
    matrix->at(i)->at(j) = index;
}

void VectorMatrix::Print()
{
    for (unsigned int i = 0; i < this->m; i++)
    {
        for (unsigned int j = 0; j < n; j++)
        {
            std::cout << "   " << matrix->at(i)->at(j);
        }
        std::cout << std::endl;
    }
}
