#include "filter.h"
#include <algorithm>
Filter::Filter(int _sizeX, int _sizeY, int _anchorX, int _anchorY, double* _values, std::string name) :
    name(name), sizeX(_sizeX), sizeY(_sizeY), anchorX(_anchorX), anchorY(_anchorY)
{
    values = new double[sizeX*sizeY];
    std::copy(_values, _values + sizeX*sizeY, values);
}

Filter::~Filter() {
    delete values;
}

int Filter::getX() { return sizeX;}
int Filter::getY() {return sizeY;}
int Filter::getAX() {return anchorX;}
int Filter::getAY() {return anchorY;}
double* Filter::getValues() {return values;}
