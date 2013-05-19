#ifndef LOGIC_HPP_INCLUDED
#define LOGIC_HPP_INCLUDED

#include <string>
#include <fstream>
#include <stdio.h>

using namespace std;

const double PSI = 0;
const double MU = 0.01;
const double E = 0.1;
const double MC = 0;
const double U = 0.01;
const double MAX_COUNT = 30;

const int H0(15), H1(10), H2(20);
const int A(H0), B(H0+H1);
const int L0(400), L1(H2*2), L2(L0 + L1);


struct Cell {
    enum { NOT_CALC, CALC, TUBE, OPEN_BORDER };
    float u, v, ksi[2], psi[2];
    char type;
    Cell() {};
};

class Schema {
    Cell ** grid;
    int XSize, YSize;
    double dx, dy, dt;


    ofstream psiOut, ksiOut;
    ofstream vOut, uOut;

public:
    Schema(int X, int Y, double dx, double dy, double dt);
    virtual void setGeometry();

    void clear();
    void startConditions();
    void borderConditions();

    void calcKsi();
    void calcPsi();
    void calcNewLayer();
    void recalcOldLayer();

    void writeTube(string FileName);
    void outResult();
    void openOuts();
    void closeOuts();

    virtual ~Schema();
};

#endif // LOGIC_HPP_INCLUDED
