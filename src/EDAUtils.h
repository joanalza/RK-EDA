/* 
 * File:   EDAUtils.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:37
 */

#ifndef EDAUTILS_H
#define	EDAUTILS_H
#include <vector>       // std::vector
#include <random>       // std::vector
using namespace std;
#include "PFSP.h"
#include "RK.h"
class EDAUtils {
public:
     EDAUtils();
    virtual ~EDAUtils();
    bool myfunction(RK* i, RK* j);
    struct myclass;
    vector<RK*> sortPopulation(vector<RK*> pop);
    RK* getBestSolutionMin(vector<RK*> pop);
    double getPopulationAverageFitness(vector<RK*> pop);
//    RK* getBestSolutionMax(vector<RK*> pop);
    double* getPM(vector<RK*> currentPopulation, int truncSize, int pSize);
    double* getChild(double* currentModel, double stdev, int pSize);
//    vector<double*> getPopulation(double* currentModel, double stdev, int probSize, int populationSize);
//	vector<RK*> initialPopulationRK(int probSize, int populationSize, string fileName);
//    vector<RK*> getPopulationRK(double* currentModel, double stdev, int probSize, int populationSize);
    double Mean(double* array, int tSize);
    int* randomKeyToAL(double priorities[], int pSize);

    double Variance(double* array, int num);
    string perm2str(int* p, int n);

private:

};


#endif	/* EDAUTILS_H */

