#ifndef LOGIC_HPP_INCLUDED
#define LOGIC_HPP_INCLUDED

#include <string>
#include <fstream>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

const double PSI = 0;
const double MU = 0.01;
const double E = 0.1;
const double MC = 0;
const double U = 0.01;
const double MAX_COUNT = 30;

const double ANGLE = 30;
const int H0(15), H1(30), H2(15); 
const int A(H0), B(H0);
const int L0(400), L1((H1)*(cos(ANGLE*M_PI/180.)/sin(ANGLE*M_PI/180.))), L2(20), L3(L0 + L1*2 + L2);


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
