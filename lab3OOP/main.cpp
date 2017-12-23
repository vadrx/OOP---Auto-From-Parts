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
    Engine en(4500);
    double gears[] = {2.7, 1.5, 1.0, 0.67};
    Transmission transm(4, vector<double>(gears, gears + 4), en);
    transm.mainGear = 4.2;
    Accelerator pedal(en);
    Wheels wh(15, transm);
    Car car1("Audi 80", 150, transm, wh, pedal, en);
    Driver dr1("Andrey", car1);
    dr1.speedUpCar(fout);
    return 0;
}
