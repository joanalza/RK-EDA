/*
 * DPFSP.h
 *
 *  Created on: 10 Apr 2018
 *      Author: 1715818
 */

#ifndef DPFSP_H_
#define DPFSP_H_

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdio.h>

#include "DynPermP.h"
#include "Tools.h"
#include "RK.h"
using std::istream;
using std::ostream;
using namespace std;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;


class DPFSP : public DynPermP
{

public:

	DPFSP();
	virtual ~DPFSP();

	int ReadInstance(string filename);
	void ReadDynamic(string dynamic);
	double EvaluateFSPMakespan(int * genes);
	double Evaluate(int * genes);
	int getNumberofEvaluation();
	int* getIdentityPermutation(int period);
	int GetProblemSize();
	double getChangeStep(int changePeriod);
	int getNumOfChanges();
	string getDistance(string dynamic);
	string getDistanceType(string dynamic);
	string getDistanceMagnitude(string dynamic);
	void setIdentityPermutationChanges();
	//bool changeIdentityPermutation(int fes, int maxfes);
	bool detectChange(RK *perm1, RK *perm2);

	int m_jobs;
	int m_machines;
	int **m_processing_matrix;
	int m_evaluations;
//	Tools m_t;

//	string m_dynamicProfilePath;
//	int m_nextChangeIndex;
//	int m_changes;
//	int *m_identityPermutation;
//	int **m_identityPermutations;
//	double *m_identityChangesPercentage; // Percentage of the maxFes at which a change happens

private:

};

#endif /* DPFSP_H_ */
