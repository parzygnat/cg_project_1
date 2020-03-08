#include "filter.h"
#include <algorithm>
Filter::Filter(int _sizeX, int _sizeY, int _anchorX, int _anchorY, std::vector<double> _values, std::string name) :
    name(name), sizeX(_sizeX), sizeY(_sizeY), anchorX(_anchorX), anchorY(_anchorY)
{
    values = new double[sizeX*sizeY];
    std::copy(_values.begin(), _values.begin() + sizeX*sizeY, values);
}

Filter::~Filter() {
}

int Filter::getX() { return sizeX;}
int Filter::getY() {return sizeY;}
int Filter::getAX() {return anchorX;}
int Filter::getAY() {return anchorY;}
double* Filter::getValues() {return values;}
