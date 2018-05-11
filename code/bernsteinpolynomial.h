#ifndef BERNSTEINPOLYNOMIAL_H
#define BERNSTEINPOLYNOMIAL_H

#include <matrix.h>


class BernsteinPolynomial
{
	public:
		BernsteinPolynomial(unsigned int degree, unsigned int resolution);
		~BernsteinPolynomial();

		double Get(unsigned int i, unsigned int t) const { return this->data->getAt(i, t); }

	private:
		unsigned int resolution;
		unsigned int degree;
		Matrix<double>* data;

		void precalcData();
};

#endif // BERNSTEINPOLYNOMIAL_H
