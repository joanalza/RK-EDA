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
#include "AziziAdaptativeCooling.h"
#include "DPFSP.h"
#include "DynPermP.h"
using namespace std;

class RKEDA {
public:

	RKEDA();
    RKEDA(int popSize, string problemPath, string dynamicPath, int FEs, int truncSize, int elitism, string results, unsigned long theseed, int restart);
	virtual ~RKEDA();

//    void setResultsPath(std::string fileName, std::string dynamicName, double minTemp, double heating, int run);
    void runAlgorithm(double minTemp, double heating);

    int m_problemSize;
    int m_populationSize;
    int m_truncationSize;
    double m_elitism;
    int m_generations;
    int m_restart;
    string m_fileName;
    string m_dynamicPath;
    string m_resultsPath;
    string m_modelPath;
    EDAUtils m_e;
//    PFSP m_fsp;
    DynPermP *m_dop;
    AziziAdaptativeCooling m_cooling;
	unsigned long m_seed;
private:

};


#endif	/* RKEDA_H */

