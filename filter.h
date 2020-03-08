#ifndef FILTER_H
#define FILTER_H
#include <string>

class Filter {
public:
    Filter(int _sizeX, int _sizeY, int _anchorX, int _anchorY, double* _values, std::string name);
    std::string name;
    int getX();
    int getY();
    int getAX();
    int getAY();
    double* getValues();
    ~Filter();
private:
    int sizeX;
    int sizeY;
    int anchorX;
    int anchorY;
    double* values;
};

#endif // FILTER_H
