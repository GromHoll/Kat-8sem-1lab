#include "logic.hpp"
#include <string>
#include <iostream>

using namespace std;

int main() {
    int t = 1000;
    Schema sh(1000, 45, 1, 1, 1);
    sh.setGeometry();
sh.borderConditions();
    string filename = "Tube.txt";
    sh.writeTube(filename);

    sh.startConditions();

    sh.openOuts();
    for(int i = 0; i <= t; i++) {
        cout << "Step: " << i << endl;
        sh.borderConditions();
        sh.calcNewLayer();
        if(i%25 == 0) {
            cout << "Write step: " << i << endl;
            sh.outResult();
        }
        sh.recalcOldLayer();
    }
    sh.closeOuts();


    return 0;
}
