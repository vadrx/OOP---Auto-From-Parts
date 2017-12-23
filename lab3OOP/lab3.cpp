//
//  lab3.cpp
//  lab3OOP
//
//  Created by Антон Волков on 22.12.2017.
//  Copyright © 2017 Антон Волков. All rights reserved.
//

#include "lab3.hpp"

Driver::Driver(string name, Car& car1){
    this->name = name;
    this->car1 = &car1;
}
Car::Car(string name, int maxSpeedCar, Transmission& transm, Wheels& wh, Accelerator& pedal, Engine& en){
    this->name = name;
    this->maxSpeedCar = maxSpeedCar;
    this->transm= &transm;
    this->wh = &wh;
    this->pedal = &pedal;
    this->en = &en;
}
Engine::Engine(double maxSpeeds){
    this->maxSpeeds = maxSpeeds;
    currSpeeds = 0;
}
Accelerator::Accelerator(Engine &en) {
    this->en = &en;
    pedalPressed = false;
    force = 0;
}
Transmission::Transmission(int countNum, const vector<double> &gears, Engine &en) : akppGears(gears)
{
    this->countNum = countNum;
    this->en = &en;
    mainGear = 0;
    gearRatio = 0;
    unionGearRatio = 0;
    currGear = 0;
    clutchPressed = false;
    speedsToUnGearRatio = 0;
}
Wheels::Wheels(int diameter, Transmission& transm){
    this->transm = &transm;
    this->diameter = diameter;
    currSpeedWheels = 0;
}
void Driver::pressPedal(ofstream &fout){
    car1->pedal->setPedalPress();
    srand((unsigned int)time(0));
    car1->pedal->setPedalForce(1 + rand() % 2);
    car1->pedal->changeSpeedsEngine(fout);
}
void Driver::unPressPedal(ofstream &fout){
    car1->pedal->setPedalUnpress();
    car1->pedal->setPedalForce(0);
    car1->pedal->changeSpeedsEngine(fout);
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
            en->downSpeeds();
            fout << "Педаль газа отпущена, обороты уменьшаются до " << en->getCurrSpeeds()*1000 <<endl;
        }
            break;
        case 1:{
            en->upSpeeds();
            fout << "Педаль газа нажата до половины, обороты увеличиваются до " << en->getCurrSpeeds()*1000 <<endl;
        }
            break;
        case 2:{
            en->upSpeeds();
            en->upSpeeds();
            fout << "Педаль газа нажата до максимума, обороты увеличиваются до " << en->getCurrSpeeds()*1000 <<endl;
        }
            break;
        default:
            break;
    }
    return en->getCurrSpeeds();
}
int Transmission::gearShift(ofstream &fout){
    if (en->getCurrSpeeds()*1000 > 2000) {
        en->downSpeeds();
        fout << "Обороты снижены до " << en->getCurrSpeeds()*1000 <<endl;
    }
    clutchPressed = true;
    fout << "Выжато сцепление" << endl;
    currGear++;
    fout << "Передача переключена на " << currGear << endl;
    getGearRatio();
    clutchPressed = false;
    fout << "Сцепление отпущено" << endl;
    en->downSpeeds();
    fout << "Обороты снижены до " << en->getCurrSpeeds()*1000 <<endl;
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
    speedsToUnGearRatio = en->getCurrSpeeds() / setUnionGearRatio();
    return speedsToUnGearRatio;
}
void Wheels::getCurrSpeedWh(ofstream &fout){
    fout<<"Текущая скорость на колесах " << currSpeedWheels <<" км/ч " << endl;
}
int Wheels::setCurrSpeedWh(){
   return currSpeedWheels = transm->setSpeedsToUnGearRatio() * diameter * 20;
}
void Driver::speedUpCar(ofstream &fout){
    car1->wh->getCurrSpeedWh(fout);
        while (car1->transm->getCurrGear() < car1->transm->countNum) {
            pressPedal(fout); //нажать педаль газа
            while (car1->en->getCurrSpeeds() > car1->en->maxSpeeds/2000) {
                unPressPedal(fout);
                car1->transm->gearShift(fout);
                car1->wh->setCurrSpeedWh();
                car1->wh->getCurrSpeedWh(fout);
            }
        }
}
