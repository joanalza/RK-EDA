//#include "stdafx.h"
#include <iostream>
using namespace std;
#include "RK.h"
#include "Tools.h"



//int* permutation= new int[IND_SIZE];

RK::RK(){

}

RK::RK(double randomkeys1[], int psize) {
	pSize = psize;
	//randomkeys = new double[psize];
	//permutation = new int[psize];
	randomkeys = randomkeys1;
	/*for (int i = 0; i < psize; i++) {
		randomkeys[i] = randomkeys1[i];
	}*/
}

RK::~RK()
{
	delete[] randomkeys;
	delete[] permutation;

}

RK* RK::Clone()
{
	//RK rk1(randomkeys, pSize);
	RK*  ind = new RK(randomkeys, pSize);
	ind->setFitness(fitness);
	ind->setPermutation(permutation);
	return ind;
}

RK* RK::Clone2()
{
	double* newrandomkeys = new double[pSize];
	for (int i = 0; i < pSize; i++) {
		newrandomkeys[i] = randomkeys[i];
	}
	RK*  ind = new RK(newrandomkeys, pSize);
	
	int* newperm = new int[pSize];
	for (int i = 0; i < pSize; i++) {
		newperm[i] = permutation[i];
	}

	ind->setPermutation(newperm);
	ind->setFitness(fitness);

	return ind;
}


double RK::getFitness() {
	return fitness;
}

void RK::setFitness(double fitness1) {
	fitness = fitness1;
}

void RK::setProblemSize(int size) {
	pSize = size;
}

int RK::getProblemSize() {
	return pSize;
}

int* RK::getPermutation() {
	/*int* perm = new int[pSize];

	for (int i = 0; i < pSize; i++) {
		perm[i] = permutation[i];
	}
	return perm;*/
	return permutation;
}

void RK::setPermutation(int* perm) {

	permutation = perm;
	// for (int i = 0; i < pSize; i++) {
	 //    permutation[i] = perm[i];
	// }
}

double* RK::copyGene() {
	double* randomkeys1 = new double[pSize];
	for (int i = 0; i < pSize; i++) {
		randomkeys1[i] = randomkeys[i];
	}
	return randomkeys1;
}

RK RK::copyOf() {

	RK rk1(randomkeys, pSize);
	rk1.setFitness(fitness);
	rk1.setPermutation(permutation);
	return rk1;
}

void RK::normalise() {
	for (int i = 0; i < pSize; i++) {
		//            AL[i] = (double)(ranks[i]-1)/(size-1);
		randomkeys[permutation[i]] = (double)((double)i / (pSize - 1));
	}
	//randomkeys = normaliseRanks(permutation);
}
double* RK::getNormalisedRKs()
{
	return randomkeys;
}

// double* RK::normaliseRanks(int ranks[]) {
//
//    //double* AL[pSize];
//    double* AL= new double[pSize];
//    
//    for (int i = 0; i < pSize; i++) {
//        //            AL[i] = (double)(ranks[i]-1)/(size-1);
//        AL[ranks[i]] = (double)((double)i /(pSize - 1));
//    }
//    //             System.out.println("normalised: " + Arrays.toString(AL));
//    return AL;
//}
// 

string RK::getPermutationAsString() {
	string out = t.perm2str(permutation, pSize);

	/*string out = "";
	for (int i = 0; i < pSize; i++) {
		out = out + permutation[i];
		if (i < pSize - 1)
			out = out+ ",";
	}*/

	return out;
}

string RK::getRandomKeyAsString() {
	string out = t.rk2str(randomkeys, pSize);

	/*string out = "";
	for (int i = 0; i < pSize; i++) {
		out = out + permutation[i];
		if (i < pSize - 1)
			out = out+ ",";
	}*/

	return out;
}

