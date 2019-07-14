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
    struct byFitness;
    struct byPermutation;
    vector<RK*> sortPopulation(vector<RK*> pop);
    void sortPop(vector<RK*> pop);
    bool isSortedPopulation(vector<RK*> pop);
    bool areRepeatedInPopulation(vector<RK*> pop);
    RK* getBestSolutionMin(vector<RK*> pop);
    bool popContainsPermutation(vector<RK*> pop, RK *x);
    double getPopulationAverageFitness(vector<RK*> pop);
//    RK* getBestSolutionMax(vector<RK*> pop);
    double* getPM(vector<RK*> currentPopulation, int truncSize, int pSize);
    double* getChild(double* currentModel, double stdev, int pSize);
//    vector<double*> getPopulation(double* currentModel, double stdev, int probSize, int populationSize);
//	vector<RK*> initialPopulationRK(int probSize, int populationSize, string fileName);
//    vector<RK*> getPopulationRK(double* currentModel, double stdev, int probSize, int populationSize);
    double Mean(double* array, int tSize);
    int* randomKeyToAL(double priorities[], int pSize);
    RK* minimumSolution(RK* perm1, RK* perm2);
    RK* maximumSolution(RK* perm1, RK* perm2);

    double Variance(double* array, int num);
    double lowest(double* array, int size);
    double highest(double* array, int size);
    string perm2str(int* p, int n);

private:

};


#endif	/* EDAUTILS_H */

