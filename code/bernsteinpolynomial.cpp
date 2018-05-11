#include "bernsteinpolynomial.h"

#include <math.h>


BernsteinPolynomial::BernsteinPolynomial(unsigned int degree, unsigned int resolution)
{
	this->degree = degree;
	this->resolution = resolution;

	this->precalcData();
}

BernsteinPolynomial::~BernsteinPolynomial()
{
	delete this->data;
}

/// Source: https://ideone.com/aDJXNO
unsigned int nChoosek(unsigned int n, unsigned int k)
{
	if (k > n)
		return 0;
	if (k * 2 > n)
		k = n-k;
	if (k == 0)
		return 1;

	int result = n;
	for (unsigned int i = 2; i <= k; ++i)
	{
		result *= (n-i+1);
		result /= i;
	}
	return result;
}

void BernsteinPolynomial::precalcData()
{
	double stepSize = 1.0f / this->resolution;

	this->data = new Matrix<double>(this->degree+1, this->resolution+1);
	double val;

	for (unsigned int i = 0; i < this->degree+1; i++)
	{
		for (unsigned int t = 0; t < this->resolution+1; t++)
		{
			val = nChoosek(this->degree, i) * pow(t*stepSize, i) * pow((1.0f - t*stepSize), (this->degree-i));
			this->data->setAt(i, t, val);
		}
	}
}
