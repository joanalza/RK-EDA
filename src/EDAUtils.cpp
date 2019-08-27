/* 
 * File:   EDAUtils.cpp
 * Author: mayowaayodele
 * 
 * Created on 24 April 2017, 15:37
 */
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <iostream>
#include <random>
using namespace std;
#include "RK.h"
#include "EDAUtils.h"
#include "PFSP.h"
#include "EDAUtils.h"

//EDAUtils::EDAUtils() {
//}
//
//EDAUtils::EDAUtils(const EDAUtils& orig) {
//}

EDAUtils::~EDAUtils() {

}


EDAUtils::EDAUtils() {

}



bool EDAUtils::myfunction(RK* i, RK* j) {
    return (i->getFitness() < j->getFitness());
}

struct EDAUtils::byFitness {

    bool operator()(RK* i, RK* j) {
        return (i->getFitness() < j->getFitness());
    }
} structuredByFitness;

//struct EDAUtils::byPermutation {
//	int *m_permutation;
//	byPermutation():
//	bool operator==(RK *i) const {
//        return (Tools::areEqual(m_permutation, i->getPermutation(), i->m_pSize, i->m_pSize));
//    }
//} structuredByPermutation;

vector<RK*> EDAUtils::sortPopulation(vector<RK*> pop) {

    std::sort(pop.begin(), pop.end(), structuredByFitness);  // O(nlogn)
    return pop;
}

void EDAUtils::sortPop(vector<RK*> pop) {
    std::sort(pop.begin(), pop.end(), structuredByFitness);
}

bool EDAUtils::isSortedPopulation(vector<RK*> pop){
	for(int i=1; i< pop.size(); i++){
		if (pop.at(i - 1)->getFitness() > pop.at(i)->getFitness())
			return false;
	}
    return true;
}

bool EDAUtils::popContainsPermutation(vector<RK*> pop, RK *x) {
//	Tools::PrintArray(x->getPermutation(), x->m_pSize, "Generated m_permutation: ");
	for(int i=0; i< pop.size(); i++){
//		Tools::PrintArray(pop.at(i)->getPermutation(), pop.at(i)->m_pSize, "Permutation in pop: ");
		if (Tools::areEqual(pop.at(i)->getPermutation(), x->getPermutation(), pop.at(i)->m_pSize, x->m_pSize)){
			return true;
		}
	}
	return false;
}

bool EDAUtils::areRepeatedInPopulation(vector<RK*> pop){
	for (int i=0; i< pop.size();i++){
		RK *perm = pop.at(i);
		for (int j=0; j<pop.size(); j++){
			if (i!=j){
				if (Tools::areEqual(perm->getPermutation(), pop[j]->getPermutation(), perm->m_pSize, pop[j]->m_pSize))
					cout << i << "," << j << endl;
					return true;
			}
		}
	}
	return false;
}

RK* EDAUtils::getBestSolutionMin(vector<RK*> pop){
    RK *best = pop.at(0);
    for (int i = 1; i < pop.size(); i++) {
        RK *S = pop.at(i);
        if (S->getFitness() < best->getFitness()) {
            best = S->Clone();
        }
    }
    return best;
}

double EDAUtils::getPopulationAverageFitness(vector<RK*> pop){
	double *vals = new double[pop.size()];
	for(int i=0; i<pop.size(); i++){
		vals[i] = pop.at(i)->getFitness();
	}
	double avg = EDAUtils::Mean(vals, pop.size());
	delete[] vals;
	return avg;
}

void EDAUtils::getPM(vector<RK*> currentPopulation, int truncSize, int pSize, double* matrix) {
    vector<RK*> selectedGenome;
    vector<RK*> pop;
//    double* matrix = new double[m_pSize];
    pop = sortPopulation(currentPopulation);
    for (int i = 0; i < truncSize; i++) {
        selectedGenome.push_back(pop[i]);
    }
    double mean;
    for (int i = 0; i < pSize; i++) {

        mean = 0;
        double* S = new double[truncSize];
        for (int j = 0; j < truncSize; j++) {
			S[j] = selectedGenome[j]->getNormalisedRKs()[i];
        }

        mean = Mean(S, truncSize);
        matrix[i] = mean;
        delete[] S;
    }
    selectedGenome.clear();
    //selectedGenome.shrink_to_fit(); // ##C++11
	selectedGenome.swap(selectedGenome); // ##C++0x
    pop.clear();
    //pop.shrink_to_fit(); // ##C++11
	pop.swap(pop); // ##C++0x
    
//    return matrix;
	//delete[] matrix;
}

double EDAUtils::Mean(double* array, int tSize) {
    double sum = 0.0;
    for (int i = 0; i < tSize; i++) {
        sum += array[i];

    }
    sum = sum / (double) tSize;
    return sum;
}

//double EDAUtils::Variance(double* array, int num) {
//    double avg = Mean(array, num);
//    double var = 0;
//    for (int i = 0; i < num; i++) {
//        var += ((array [i] - avg) * (array [i] - avg));
//    }
//    var /= (num - 1);
//    return var;
//}

//int* EDAUtils::randomKeyToAL(double* rks, int problemSize){
//		int* AL = new int[problemSize];
//		double* ordered_rks = new double[problemSize];
//		bool found = false;
//		int i, j = 0;
//		for (i = 0; i < problemSize; i++) {
//			ordered_rks[i] = rks[i];
//		}
////		Tools::printRK(ordered_rks, problemSize);
//		sort(ordered_rks, ordered_rks + problemSize);
////		Tools::printRK(ordered_rks, problemSize);
//		for (i = 0; i < problemSize; i++) {
//			found = false;
//			while (!found) {
//				if (ordered_rks[j] == rks[i]) {
//					AL[i] = j;
//					found = true;
//					j = 0;
//				} else
//					j++;
//			}
//		}
////		Tools::printPermutation(AL, problemSize);
//		delete[] ordered_rks;
//		return AL;
//	}

RK* EDAUtils::minimumSolution(RK* sol1, RK* sol2) {
	RK *sol = new RK();
    if(sol1->getFitness() < sol2->getFitness())
    	sol = sol1->Clone();
    else
    	sol = sol2->Clone();
    return sol;
}

RK* EDAUtils::maximumSolution(RK* sol1, RK* sol2) {
	RK *sol = new RK();
    if(sol1->getFitness() > sol2->getFitness())
    	sol = sol1->Clone();
    else
    	sol = sol2->Clone();
    return sol;
}

//int EDAUtils::getNumberofEvaluations()
//{
//    return fsp.getNumberofEvaluation();
//}

string EDAUtils::perm2str(int* p, int n) {
    string s = std::to_string(static_cast<long long>(p[0]));
    for (int i = 1; i < n; i++) {
		//s += std::to_string(p[i]) + ","; // ##C++11
        s+= "," + std::to_string(static_cast<long long>(p[i])) ; // ##C++0x
    }
    return s;
}

// Function definition for lowest value
double EDAUtils::lowest(double* array, int size){
	double min; // min is of type FLOAT
	min = array[0]; // min is the 0th element

	for (int i=0; i <size;i++)
	{
		if( array[i] < min) // if i-th element is less than min
		{
			min = array[i]; // make that our new min
		}
	}
	return min;
}


// Function definition for highest value
double EDAUtils::highest(double* array, int size){
	double max; // max is of type FLOAT
	max = array[0]; // max is the 0th element

	for (int i=0; i <size;i++)
	{
		if( array[i] > max) // if i-th element is less than max
		{
			max = array[i]; // make that our new max
		}
	}
	return max;
}

