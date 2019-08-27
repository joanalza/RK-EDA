/*
 *  PBP.h
 *  RankingEDAsCEC
 *
 *  Created by Josu Ceberio Uribe on 7/11/13.
 *  Copyright 2013 University of the Basque Country. All rights reserved.
 *
 */
#ifndef __PBP_H__
#define __PBP_H__

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdio.h>

using namespace std;
using std::stringstream;
using std::string;

class DynPermP
{
public:
		
    /*
     * Auxiliary vector for inversion.
     */
    int m_numChanges;
	int* m_identityPerm;
	//int** identityPermutations;
	vector<int*> m_identityPermutations;
	int m_problemSize;
	double* m_normalisedChangeGenerations; // percentage of the maxFes at which a change happens
	int m_nextChangeIndex;
//	string m_dynamicProfilePath;

	/*
	 * The constructor.
	 */
	DynPermP();
	
	/*
	 * The destructor. It frees the memory allocated.
	 */
	virtual ~DynPermP();
	
	/*
	 * Virtual evaluation function.
	 */
	virtual double Evaluate(int*genes)=0;

//    /*
//	 * Virtual evaluation function of the inverted solution.
//	 */
//	virtual double EvaluateInv(int*genes)=0;
    
    /*
	 * Virtual instance reading function.
	 */
	virtual int ReadInstance(string filename)=0;
	virtual void ReadDynamic(string filename)=0;
    
    /*
     * Returns the size of the problem.
     */
    virtual int GetProblemSize()=0;
    virtual int getNumOfChanges()=0;

	virtual void setIdentityPermutationChanges(string dynamicfilename);

	virtual bool changeIdentityPermutation(int fes, int maxfes);

	virtual void setPbsize(int size);

	virtual string getDistance(string dynamic)=0;
	virtual string getDistanceType(string dynamic)=0;
	virtual string getDistanceMagnitude(string dynamic)=0;
	virtual double getChangeStep(int changePeriod)=0;
	virtual int* getIdentityPermutation(int period) =0;

private:
    

};
#endif
