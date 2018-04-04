/* 
 * File:   PFSP.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:30
 */

#ifndef PFSP_H
#define	PFSP_H
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

class PFSP
{

public:

	/*
	* The number of jobs of the problem.
	*/
	int JOB_NUM;

	/*
	* The number of machines of the problem.
	*/
	int MACHINE_NUM;

	/*
	* The processing times matrix.
	*/
	int **JOBPROCESSINGMATRIX;
	int **JOBPROCESSINGMATRIX2;

	/*
	* Auxiliary data tables to achieve a higher speed-up when evaluating.
	*/
	int ** jobsTimeTable_aux;

	int * genes_aux;
	int * inverted_aux;

	// The constructor. It initializes a flowshop scheduling problem from a file.
	PFSP();

	// The destructor.
	virtual ~PFSP();

	/*
	* Reads and PFSP file of Taillard Data set with the given filename.
	*/
	int ReadInstance(string filename);

	/*
	* This function evaluates the individuals for the makespan of FSP problem.
	*/
	double EvaluateFSPMakespan(int * genes);

	/*
	* This function evaluates the individuals for the total flowtime of FSP problem.
	*/
	int EvaluateFSPTotalFlowtime(int * genes);
	int getNumberofEvaluation();
	int EVALUATIONS;
	Tools t;
private:

	

};


#endif	/* PFSP_H */

