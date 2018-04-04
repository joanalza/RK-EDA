/* 
 * File:   RKEDA.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:43
 */

#ifndef RKEDA_H
#define	RKEDA_H
#include <iostream>
#include "EDAUtils.h"
using namespace std;

class RKEDA {
public:
    RKEDA(int probSize, int popSize, int trunSize, double stdev, int FEs, string filename, std::string results, unsigned long seed);
    //	void runAlgorithm(string fileName, string fileNameResults);
    void runAlgorithm1(string fileName, string fileNameResults);
    int problemSize;
    int populationSize;
    int truncationSize;
    double variance;
    string fileName;
    int fitnessEvaluations;
    string fileNameResults;
    EDAUtils e;
    PFSP fsp;
	unsigned long seed;
private:

};


#endif	/* RKEDA_H */

