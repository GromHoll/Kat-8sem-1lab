#include "logic.hpp"
#include <math.h>

using namespace std;

Schema::Schema(int X, int Y, double dx, double dy, double dt) {
    this->dx = dx;
    this->dy = dy;
    this->dt = dt;
    this->XSize = X;
    this->YSize = Y;

    this->grid = new Cell*[XSize];
    for(int i = 0; i < XSize; i++) {
        grid[i] = new Cell[YSize];
    }

	clear();
}

void Schema::setGeometry() {

    for(int i = 0; i < XSize; i++) {
        /* Line of simetry */
        grid[i][0].type = Cell::OPEN_BORDER;

        if(i < L0 || i > L2-1) {
            /* Horizontal tube */
            grid[i][H0-1].type = Cell::TUBE;
        } else if(i < L0 + H2) {
            /* Left angle side of tube */
            int j = B+i-L0;
            grid[i][j].type = Cell::TUBE;
        } else {
            /* Right angle side of tube */
            int j = B+2*H2-i+L0-1;
            grid[i][j].type = Cell::TUBE;
        }
    }

    /* Vertical side of tube */
    for(int j = H0-1; j < B; j++) {
        grid[L0][j].type = Cell::TUBE;
        grid[L2-1][j].type = Cell::TUBE;
    }
    /* Left and right side of tube */
    for(int j = 1; j < A - 1; j++) {
        grid[0][j].type = Cell::OPEN_BORDER;
        grid[XSize-1][j].type = Cell::OPEN_BORDER;
    }

    /* Fill tube */
    for(int i = 1; i < XSize-1; i++) {
        for(int j = 1; j < H0-1; j++) {
            grid[i][j].type = Cell::CALC;
        }
    }
    for(int i = L0+1; i < L2-1; i++) {
        for(int j = A-1; j < B; j++) {
            grid[i][j].type = Cell::CALC;
        }
    }

    for(int i = L0; i < L2; i++) {
        for(int j = B; j < YSize; j++) {
            if(grid[i][j].type == Cell::TUBE) {
                break;
            } else {
                grid[i][j].type = Cell::CALC;
            }
        }
    }
}

Schema::~Schema() {
    for(int i = 0; i < YSize; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Schema::writeTube(string FileName) {
    ofstream out(FileName.c_str());
    for(int j = 0; j < YSize; j++) {
        for(int i = 0; i < XSize; i++) {
            switch(grid[i][j].type) {
                case Cell::CALC:
                    out << ':';
                    break;
                case Cell::NOT_CALC:
                    out << '.';
                    break;
                case Cell::OPEN_BORDER:
                    out << '~';
                    break;
                case Cell::TUBE:
                    out << '#';
                    break;
            }
        }
        out << '\n';
    }
    out.close();
}

void Schema::clear() {
    for(int i = 0; i < XSize; i++) {
        for(int j = 0; j < YSize; j++) {
			grid[i][j].type = Cell::NOT_CALC;
			grid[i][j].ksi[0] = grid[i][j].ksi[1] = 0;
			grid[i][j].psi[0] = grid[i][j].psi[1] = 0;
			grid[i][j].u = grid[i][j].v = 0;
        }
    }
}

void Schema::startConditions() {
    for(int i = 0; i < XSize; i++) {
        for(int j = 0; j < A; j++) {
            grid[i][j].u = 4.*U*j*(A-j)/(A*A);
            grid[i][j].v = 0;
            grid[i][j].ksi[0] = 4.*U*(A-2*j)/(A*A);
            grid[i][j].psi[0] = PSI + 4.*U*(A*j*j/2.- j*j*j/3.)/(A*A);
        }
    }
}
void Schema::borderConditions() {
    for(int i = 0; i < XSize; i++) {
        /* Line of simetry */
        grid[i][0].ksi[0] = 0;
        grid[i][0].psi[0] = PSI;
        grid[i][0].v = 0;
        grid[i][0].u = U;

        if(i < L0 || i > L2-1) {
            /* Horizontal tube */
            grid[i][H0-1].psi[0] = 0;
            grid[i][H0-1].v = 0;
            grid[i][H0-1].u = 0;
            grid[i][H0-1].ksi[0] = 3.*(grid[i][H0-2].psi[0] - grid[i][H0-1].psi[0])/(dy*dy)
                                   - grid[i][H0-2].ksi[0]/2.;
        } else if(i < L0 + H2) {
            /* Left angle side of tube */
            int j = B+i-L0;
            grid[i][j].v = 0;
            grid[i][j].u = 0;
            grid[i][j].psi[0] = PSI;
            grid[i][j].ksi[0] = 3.*(grid[i+1][j-1].psi[0] - grid[i][j].psi[0])/(1.414*dx*dy)
                                - grid[i+1][j-1].ksi[0]/2.;
        } else {
            /* Right angle side of tube */
            int j = B+2*H2-i+L0-1;
            grid[i][j].v = 0;
            grid[i][j].u = 0;
            grid[i][j].psi[0] = PSI;
            grid[i][j].ksi[0] = 3.*(grid[i-1][j-1].psi[0] - grid[i][j].psi[0])/(1.414*dx*dy)
                                - grid[i-1][j-1].ksi[0]/2.;
        }
    }

    /* Vertical side of tube */
    for(int j = H0; j < B; j++) {
        grid[L0][j].psi[0] = grid[L2-1][j].psi[0] = PSI;
        grid[L0][j].v = grid[L2-1][j].v = 0;
        grid[L0][j].u = grid[L2-1][j].u = 0;

        grid[L0][j].ksi[0] = 3.*(grid[L0+1][j].psi[0] - grid[L0][j].psi[0])/(dx*dx)
                                - grid[L0+1][j].ksi[0]/2.;
        grid[L2-1][j].ksi[0] = 3.*(grid[L2-2][j].psi[0] - grid[L2-1][j].psi[0])/(dx*dx)
                                - grid[L2-2][j].ksi[0]/2.;
    }

    /* Horizontal and vertical tube's sides cross */
    grid[L0][H0-1].psi[0] = grid[L2-1][H0-1].psi[0] = PSI;
    grid[L0][H0-1].v = grid[L2-1][H0-1].v = 0;
    grid[L0][H0-1].u = grid[L2-1][H0-1].u = 0;

	/* WARNING */
    grid[L0][H0-1].ksi[0] = (grid[L0][H0-2].ksi[0] + grid[L0+1][H0-1].ksi[0])/2.;
    grid[L2-1][H0-1].ksi[0] = (grid[L2+1][H0].ksi[0] + grid[L2][H0+1].ksi[0])/2.;

    /* Andle tube's sides cross */
    grid[L0+H2-1][YSize-1].ksi[0] = 3.*grid[L0+H2-1][YSize-2].psi[0]/(dy*dy)
                                    - grid[L0+H2-1][YSize-2].ksi[0]/2.;
    grid[L2-H2][YSize-1].ksi[0] = 3.*grid[L2-H2][YSize-2].psi[0]/(dy*dy)
                                  - grid[L2-H2][YSize-2].ksi[0]/2.;

    /* Left and right side of tube */
	for(int j = 1; j < A - 1; j++) {
        grid[0][j].ksi[0] = grid[1][j].ksi[0];
        grid[XSize-1][j].ksi[0] = grid[XSize-2][j].ksi[0];

        grid[0][j].u = grid[1][j].u;
        grid[XSize-1][j].u = grid[XSize-2][j].u;

        grid[0][j].v = grid[XSize-1][j].v = 0;

        grid[0][j].psi[0] = grid[1][j].psi[0];
        grid[XSize-1][j].psi[0] = grid[XSize-2][j].psi[0];
    }
}

void Schema::calcKsi() {
    double tempX, tempY;
    for(int i = 1; i < XSize - 1; i++) {
        for(int j = 1; j < YSize - 1; j++) {
            if(grid[i][j].type == Cell::TUBE) {
                break;
            } else {
                tempX = dt*((grid[i+1][j].u*grid[i+1][j].ksi[0] - grid[i-1][j].u*grid[i-1][j].ksi[0])/(2.*dx)
                        + MU*(grid[i+1][j].ksi[0] + grid[i-1][j].ksi[0] - 2.*grid[i][j].ksi[0])/(dx*dx));
                tempY = dt*((grid[i][j+1].v*grid[i][j+1].ksi[0] - grid[i][j-1].v*grid[i][j-1].ksi[0])/(2.*dy)
                        + MU*(grid[i][j+1].ksi[0] + grid[i][j-1].ksi[0] - 2.*grid[i][j].ksi[0])/(dy*dy));
                grid[i][j].ksi[1] = tempX + tempY + grid[i][j].ksi[0];
            }
        }
    }
}
void Schema::calcPsi() {
    double betta = dx/dy;
    double bettaPow2 = betta*betta;
    int count = 0;
    double max, absCurrent;

    do {
        max = 0;
        for(int i = 1; i < XSize - 1; i++) {
            for(int j = 1; j < YSize - 1; j++) {
                if(grid[i][j].type == Cell::TUBE) {
                    break;
                } else {
                    grid[i][j].psi[1] = (grid[i+1][j].psi[0] + grid[i-1][j].psi[0]
                                        + bettaPow2*(grid[i][j+1].psi[0] + grid[i][j-1].psi[0])
                                        - dx*dx*grid[i][j].ksi[0])/(2.*(1.+bettaPow2));
                    absCurrent = fabs(grid[i][j].psi[1] - grid[i][j].psi[0]);
                    if(absCurrent > max) {
                        max = absCurrent;
                    }
                }
            }
        }

        for(int i = 0; i < XSize; i++) {
            for(int j = 0; j < YSize; j++) {
                grid[i][j].psi[0] = grid[i][j].psi[1];
            }
        }

        if(++count > MAX_COUNT) {
            break;
        }
    } while(max > E);
}
void Schema::calcNewLayer() {
    calcKsi();
    calcPsi();
}

void Schema::recalcOldLayer() {
    for(int i = 1; i < XSize - 1; i++) {
        for(int j = 1; j < YSize - 1; j++) {
            if(grid[i][j].type == Cell::TUBE) {
                break;
            } else {
                grid[i][j].u = (grid[i][j+1].psi[0] - grid[i][j-1].psi[0])/(2.*dy);
                grid[i][j].v = -(grid[i+1][j].psi[0] - grid[i-1][j].psi[0])/(2.*dx);
            }
        }
    }
    for(int i = 1; i < XSize - 1; i++) {
        for(int j = 1; j < YSize - 1; j++) {
            if(grid[i][j].type == Cell::TUBE) {
                break;
            } else {
                grid[i][j].ksi[0] = grid[i][j].ksi[1];
                grid[i][j].psi[0] = grid[i][j].psi[1];
            }
        }
    }
}

void Schema::outResult() {
    for(int j = 0; j < YSize; j++) {
        for(int i = 0; i < XSize; i++) {
            psiOut << grid[i][j].psi[0] << ' ';
            ksiOut << grid[i][j].ksi[0] << ' ';
        }
        psiOut << endl;
        ksiOut << endl;
    }
    for(int j = 0; j < YSize; j+=3) {
        for(int i = 0; i < XSize; i+=10) {
            uOut << grid[i][j].u << ' ';
            vOut << grid[i][j].v << ' ';
        }
        uOut << endl;
        vOut << endl;
    }
}
void Schema::openOuts() {
    psiOut.open("./psi.txt");
    ksiOut.open("./ksi.txt");
    vOut.open("./v.txt");
    uOut.open("./u.txt");
}
void Schema::closeOuts() {
    psiOut.close();
    ksiOut.close();
    vOut.close();
    uOut.close();
}
