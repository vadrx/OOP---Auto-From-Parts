//
//  main.cpp
//  lab3OOP
//
//  Created by Антон Волков on 22.12.2017.
//  Copyright © 2017 Антон Волков. All rights reserved.
//
#include "lab3.hpp"

int main() {
    ofstream fout("out.log");
    Engine valve8(4500);
    double gears[] = {2.7, 1.5, 1.0, 0.67};
    Transmission transm(4, vector<double>(gears, gears + 4), valve8);
    transm.mainGear = 4.2;
    Accelerator pedal(valve8);
    Wheels r15(15, transm);
    Car au80("Audi 80", 150, transm, r15, pedal, valve8);
    Driver dr1("Andrey", au80);
    dr1.speedUpCar(fout);
    return 0;
}
