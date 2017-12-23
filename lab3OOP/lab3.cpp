//
//  lab3.cpp
//  lab3OOP
//
//  Created by Антон Волков on 22.12.2017.
//  Copyright © 2017 Антон Волков. All rights reserved.
//

#include "lab3.hpp"

Driver::Driver(string name, Car& au80){
    this->name = name;
    this->au80 = &au80;
}
Car::Car(string name, int maxSpeedCar, Transmission& akpp4, Wheels& r15, Accelerator& pedal, Engine& valve8){
    this->name = name;
    this->maxSpeedCar = maxSpeedCar;
    this->akpp4= &akpp4;
    this->r15 = &r15;
    this->pedal = &pedal;
    this->valve8 = &valve8;
}
Engine::Engine(double maxSpeeds){
    this->maxSpeeds = maxSpeeds;
    currSpeeds = 0;
}
Accelerator::Accelerator(Engine &valve8) {
    this->valve8 = &valve8;
    pedalPressed = false;
    force = 0;
}
Transmission::Transmission(int countNum, const vector<double> &gears, Engine &valve8) : akppGears(gears)
{
    this->countNum = countNum;
    this->valve8 = &valve8;
    mainGear = 0;
    gearRatio = 0;
    unionGearRatio = 0;
    currGear = 0;
    clutchPressed = false;
    speedsToUnGearRatio = 0;
}
Wheels::Wheels(int diameter, Transmission& akpp4){
    this->akpp4 = &akpp4;
    this->diameter = diameter;
    currSpeedWheels = 0;
}
void Driver::pressPedal(ofstream &fout){
    au80->pedal->setPedalPress();
    srand((unsigned int)time(0));
    au80->pedal->setPedalForce(1 + rand() % 2);
    au80->pedal->changeSpeedsEngine(fout);
}
void Driver::unPressPedal(ofstream &fout){
    au80->pedal->setPedalUnpress();
    au80->pedal->setPedalForce(0);
    au80->pedal->changeSpeedsEngine(fout);
}

double Engine::getCurrSpeeds(){
    return currSpeeds;
}
double Engine::upSpeeds(){
    return currSpeeds++;
}
double Engine::downSpeeds(){
    return currSpeeds--;
}
void Accelerator::setPedalPress(){
    pedalPressed = true;
}
void Accelerator::setPedalUnpress(){
    pedalPressed = false;
}
void Accelerator::setPedalForce(int pressForce){
    force = pressForce;
}
double Accelerator::changeSpeedsEngine(ofstream &fout){
    switch (force) {
        case 0:{
            valve8->downSpeeds();
            fout << "Педаль газа отпущена, обороты уменьшаются до " << valve8->getCurrSpeeds()*1000 <<endl;
        }
            break;
        case 1:{
            valve8->upSpeeds();
            fout << "Педаль газа нажата до половины, обороты увеличиваются до " << valve8->getCurrSpeeds()*1000 <<endl;
        }
            break;
        case 2:{
            valve8->upSpeeds();
            valve8->upSpeeds();
            fout << "Педаль газа нажата до максимума, обороты увеличиваются до " << valve8->getCurrSpeeds()*1000 <<endl;
        }
            break;
        default:
            break;
    }
    return valve8->getCurrSpeeds();
}
int Transmission::gearShift(ofstream &fout){
    if (valve8->getCurrSpeeds()*1000 > 2000) {
        valve8->downSpeeds();
        fout << "Обороты снижены до " << valve8->getCurrSpeeds()*1000 <<endl;
    }
    clutchPressed = true;
    fout << "Выжато сцепление" << endl;
    currGear++;
    fout << "Передача переключена на " << currGear << endl;
    getGearRatio();
    clutchPressed = false;
    fout << "Сцепление отпущено" << endl;
    valve8->downSpeeds();
    fout << "Обороты снижены до " << valve8->getCurrSpeeds()*1000 <<endl;
    return currGear;
}
int Transmission::getCurrGear(){
    return currGear;
}
double Transmission::getGearRatio(){
    switch (currGear) {
        case 1:
            gearRatio = akppGears[0]; //2.7
            break;
        case 2:
            gearRatio = akppGears[1]; // 1.5;
            break;
        case 3:
            gearRatio = akppGears[2]; // 1.0;
            break;
        case 4:
            gearRatio = akppGears[3]; //0.67;
            break;
        default:
            break;
    }
    return gearRatio;
}
double Transmission::setUnionGearRatio(){
    unionGearRatio = mainGear * getGearRatio();
    return unionGearRatio;
}
double Transmission::setSpeedsToUnGearRatio(){
    speedsToUnGearRatio = valve8->getCurrSpeeds() / setUnionGearRatio();
    return speedsToUnGearRatio;
}
void Wheels::getCurrSpeedWh(ofstream &fout){
    fout<<"Текущая скорость на колесах " << currSpeedWheels <<" км/ч " << endl;
}
int Wheels::setCurrSpeedWh(){
   return currSpeedWheels = akpp4->setSpeedsToUnGearRatio() * diameter * 20;
}
void Driver::speedUpCar(ofstream &fout){
    au80->r15->getCurrSpeedWh(fout);
        while (au80->akpp4->getCurrGear() < au80->akpp4->countNum) {
            pressPedal(fout); //нажать педаль газа
            while (au80->valve8->getCurrSpeeds() > au80->valve8->maxSpeeds/2000) {
                unPressPedal(fout);
                au80->akpp4->gearShift(fout);
                au80->r15->setCurrSpeedWh();
                au80->r15->getCurrSpeedWh(fout);
            }
        }
}
