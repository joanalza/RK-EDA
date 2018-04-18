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
using namespace std;

class RKEDA {
public:

    RKEDA(int popSize, std::string problemPath, std::string dynamicPath, int FEs, int truncSize, int elitism, std::string results, unsigned long theseed);

//    void setResultsPath(std::string fileName, std::string dynamicName, double minTemp, double heating, int run);
    void runAlgorithm(double minTemp, double heating);

    int m_problemSize;
    int m_populationSize;
    int m_truncationSize;
    double m_elitism;
    int m_FEs;
    string m_fileName;
    string m_dynamicPath;
    string m_resultsPath;
    EDAUtils m_e;
    PFSP m_fsp;
    DPFSP m_dfsp;
    AziziAdaptativeCooling m_cooling;
	unsigned long m_seed;
private:

};


#endif	/* RKEDA_H */

