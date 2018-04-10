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

	int m_jobs;
	int m_machines;
	int **m_processing_matrix;
	int m_evaluations;
	Tools m_t;
private:

};

#endif /* DPFSP_H_ */
