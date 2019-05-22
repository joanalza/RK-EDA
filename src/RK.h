/* 
 * File:   RK.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:27
 */
#include "Tools.h"

#ifndef RK_H
#define	RK_H
class RK {
public:
// int pSize;
//double* randomkeys;
//double fitness;
//int* permutation;
	RK();
    RK(double randomkeys1[], int psize);
  
	~RK();
   
double getFitness();

void setFitness(double fitness1);

void setProblemSize(int size);

int getProblemSize();

int* getPermutation();

void setPermutation(int* perm);

double* copyGene();

string getPermutationAsString();

string getRandomKeyAsString();

RK copyOf();

void normalise();
 double* getNormalisedRKs();
double* normaliseRanks(int ranks[]);
 RK* Clone();
 RK* Clone2();
 int pSize;
 double* randomkeys;
 double fitness;
 int* permutation;
 Tools t;
private:


};


#endif	/* RK_H */

