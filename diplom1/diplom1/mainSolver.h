#pragma once
#include <string>
#include <stdio.h>

using namespace std;

class mainSolver
{
protected:
    double FaceArea;
    double k;
    double delt;
public:
    double** T;
    double** T_new;
    int n;
    int jmax;
    double dh;

    bool create(string filename_);

    void RunPhysic();
    void RunPhysicWithNew();

    //Insulated
    void RunPhysic2();
};