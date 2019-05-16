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
#include "Tools.h"
using std::istream;
using std::ostream;
using namespace std;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;


class DPFSP
{

public:

	DPFSP();
	virtual ~DPFSP();

	int ReadTaillardInstance(string filename, string dynamic);
	double EvaluateFSPMakespan(int * genes);
	int EvaluateFSPTotalFlowtime(int * genes);
	int getNumberofEvaluation();
	int* getIdentityPermutation(int period);
	int getProblemSize();
	double getChangeStep(int changePeriod);
	string getDistance(string dynamic);
	string getDistanceType(string dynamic);
	string getDistanceMagnitude(string dynamic);
	void setIdentityPermutationChanges();
	bool changeIdentityPermutation(int fes, int maxfes);

	int m_jobs;
	int m_machines;
	int **m_processing_matrix;
	int m_evaluations;
	Tools m_t;

	string m_dynamicProfilePath;
	int m_nextChangeIndex;
	int m_changes;
	int *m_identityPermutation;
	int **m_identityPermutations;
	double *m_idenityChangesPercentage; // Percentage of the maxFes at which a change happens

private:

};

#endif /* DPFSP_H_ */
