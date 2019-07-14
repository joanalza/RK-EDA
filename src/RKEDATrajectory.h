/* 
 * File:   RKEDATrajectory.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:43
 */

#ifndef RKEDATRAJECTORY_H
#define	RKEDATRAJECTORY_H
#include <iostream>
#include "EDAUtils.h"
#include "AziziAdaptativeCooling.h"
#include "DPFSP.h"
#include "DynPermP.h"
using namespace std;

class RKEDATrajectory {
public:

	RKEDATrajectory(int popSize, std::string problemPath, std::string dynamicPath, int FEs, int truncSize, int elitism, std::string results, std::string modelPath, unsigned long theseed, int restart, std::string modelPermutation);

//    void setResultsPath(std::string fileName, std::string dynamicName, double minTemp, double heating, int run);
    void runAlgorithm(double minTemp, double heating);
    void readModePermutation(string file, int idex);
    void readModelPermutation(string File);

    int m_problemSize;
    int m_populationSize;
    int m_truncationSize;
    double m_elitism;
    int m_generations;
    int m_restart;
    int** m_modelPerms;

    string m_fileName;
    string m_dynamicPath;
    string m_resultsPath;
    string m_modelPath;
    string m_modelPermFile;
    EDAUtils m_e;
    PFSP m_fsp;
    DynPermP *m_dop;
    AziziAdaptativeCooling m_cooling;
	unsigned long m_seed;

	int **m_modePermutations;
private:

};


#endif	/* RKEDA_H */

