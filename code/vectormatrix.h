#ifndef VECTORMATRIX_H
#define VECTORMATRIX_H

#include <vector>


class VectorMatrix
{
public:
    VectorMatrix(unsigned int m, unsigned int n);
    ~VectorMatrix();

    unsigned int getM() const { return m; }
    unsigned int getN() const { return n; }

    int getAt(unsigned int i, unsigned int j) const { return this->matrix->at(i)->at(j); }
    void setAt(unsigned int i, unsigned int j, const int index);

    void Print();

private:
    unsigned int m;
    unsigned int n;

    std::vector<std::vector<int>*>* matrix;
};

#endif // VECTORMATRIX_H
