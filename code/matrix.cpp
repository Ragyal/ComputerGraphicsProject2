#include "matrix.h"

#include <iostream>


template<class T>
Matrix<T>::Matrix(unsigned int m, unsigned int n)
{
    this->m = m;
    this->n = n;


	this->matrix = new std::vector<std::vector<T>*>(m);
    for (unsigned int i = 0; i < m; i++)
	{
		matrix->at(i) = new std::vector<T>(n);
    }
}

template<class T>
Matrix<T>::~Matrix()
{
    for (unsigned int i = 0; i < matrix->size(); i++)
    {
        delete matrix->at(i);
    }
    delete matrix;
}

template<class T>
void Matrix<T>::setAt(unsigned int i, unsigned int j, const T value)
{
	matrix->at(i)->at(j) = value;
}

template<class T>
void Matrix<T>::Print()
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

template class Matrix<unsigned int>;
template class Matrix<double>;

