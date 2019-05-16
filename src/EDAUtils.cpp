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

struct EDAUtils::myclass {

    bool operator()(RK* i, RK* j) {
        return (i->getFitness() < j->getFitness());
    }
} myobject;

vector<RK*> EDAUtils::sortPopulation(vector<RK*> pop) {

    std::sort(pop.begin(), pop.end(), myobject);
    return pop;
}

RK* EDAUtils::getBestSolutionMin(vector<RK*> pop){
    RK *best = pop.at(0);
    for (int i = 1; i < pop.size(); i++) {
        RK *S = pop.at(i);
        if (S->getFitness() < best->getFitness()) {
            best = S->Clone2();
        }
    }
    return best->Clone2();
}

double EDAUtils::getPopulationAverageFitness(vector<RK*> pop){
	double *vals = new double[pop.size()];
	for(int i=0; i<pop.size(); i++){
		vals[i] = pop.at(i)->getFitness();
	}
	return EDAUtils::Mean(vals, pop.size());
}

double* EDAUtils::getPM(vector<RK*> currentPopulation, int truncSize, int pSize) {
    vector<RK*> selectedGenome;
    vector<RK*> pop;
    double* matrix = new double[pSize];
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
    
    return matrix;
}

double* EDAUtils::getChild(double* currentModel, double stdev, int pSize) {
    //        double[] currentModel = Arrays.copyOf(currentModel1,currentModel1.length );
    double* childAct = new double[pSize];
    //std::default_random_engine generator; // ##C++11
	std::minstd_rand generator; // ##C++0x

    for (int x = 0; x < pSize; x++) {
        //             double r2 = r.nextGaussian() * currentModel[x][1] + currentModel[x][0];
        double mean = currentModel[x];

        std::normal_distribution<double> distribution(mean, stdev);
        double r2 = distribution(generator);
//        cout << r2 << endl;
        childAct[x] = r2;

    }
    return childAct;
}

//vector<double*> EDAUtils::getPopulation(double* currentModel, double stdev, int probSize, int populationSize) {
//    //        double[] currentModel = Arrays.copyOf(currentModel1,currentModel1.length );
//
//    vector<double*> population;
//    std::default_random_engine generator;
//    for (int i = 0; i < populationSize; i++) {
//        double* childAct = new double[probSize];
//        for (int x = 0; x < probSize; x++) {
//            //             double r2 = r.nextGaussian() * currentModel[x][1] + currentModel[x][0];
//            double mean = currentModel[x];
//
//            std::normal_distribution<double> distribution(mean, stdev);
//            double r2 = distribution(generator);
////            cout << r2 << endl;
//            childAct[x] = r2;
//
//        }
//        population.push_back(childAct);
//        delete[] childAct;
//    }
//    return population;
//}
//
//vector<RK*> EDAUtils::getPopulationRK(double* currentModel, double stdev, int probSize, int populationSize) {
//    //        double[] currentModel = Arrays.copyOf(currentModel1,currentModel1.length );
//
//    vector<RK*> population;
//    //std::default_random_engine generator;
//	std::random_device generator;
//    for (int i = 0; i < populationSize; i++) {
//
//        double* childAct = new double[probSize];
//        for (int x = 0; x < probSize; x++) {
//            //             double r2 = r.nextGaussian() * currentModel[x][1] + currentModel[x][0];
//            double mean = currentModel[x];
//
//            std::normal_distribution<double> distribution(mean, stdev);
//            double r2 = distribution(generator);
//            childAct[x] = r2;
//
//        }
//        RK child(childAct, probSize);
//        child.setPermutation(randomKeyToAL(childAct, probSize));
//        child.normalise();
//        child.setFitness(fsp.EvaluateFSPTotalFlowtime(child.getPermutation()));
//        population.push_back(child.Clone());
//           if (i==0)
//        {
//            bestSolution = child.Clone();
//        }
//        else
//        {
//            if (bestSolution->getFitness() > child.getFitness())
//            {
//                bestSolution = child.Clone();
//            }
//        }
//          delete[] childAct;
//    }
//    return population;
//}


//vector<RK*> EDAUtils::initialPopulationRK(int probSize, int populationSize, string fileName) {
//	
//	fsp.ReadInstance(fileName);
//    vector<RK*> population;
//    for (int i = 0; i < populationSize; i++) {
//        
//        double* childAct = new double[probSize];
//        for (int x = 0; x < probSize; x++) {
//          double  r = ((double) rand() / (RAND_MAX));
//            childAct[x] = r;
//
//        }
//        RK child(childAct, probSize);
//        child.setPermutation(randomKeyToAL(childAct, probSize));
//        child.normalise();
//        child.setFitness(fsp.EvaluateFSPTotalFlowtime(child.getPermutation()));
//        population.push_back(child.Clone());
//        if (i==0)
//        {
//            bestSolution = child.Clone();
//        }
//        else
//        {
//            if (bestSolution->getFitness() > child.getFitness())
//            {
//                bestSolution = child.Clone();
//            }
//        }
//          delete[] childAct;
//    }
//    
//    return population;
//}

int* EDAUtils::randomKeyToAL(double* priorities, int pSize) {

	double* p = new double[pSize];
    vector<int> AL;

    for (int i = 0; i < pSize; i++) {
		p[i] = priorities[i];
    }
	sort(p, p + pSize);
	int k = 0;

	for (int i = 0; i < pSize; i++) {
		for (int j = 0; j < pSize; j++) {
            if (((p[i] == priorities[j]) && AL.empty() == true) || ((p[i] == priorities[j]) && ((std::find(AL.begin(), AL.end(), j) != AL.end()) == false))) {

                AL.push_back(j);
				k++;
            }

            if (k == pSize) {
                goto outer;
            }

        }

    }

outer:

    // cout << perm2str(AL, pSize) << endl;
    int* AL1 = new int[AL.size()];
    for (unsigned int i = 0; i < AL.size(); i++) {
        AL1[i] = AL[i];
    }
    AL.clear();
    //AL.shrink_to_fit(); // ##C++11
	AL.swap(AL); // ##C++0x
    delete[] p;
    return AL1;
}

double EDAUtils::Mean(double* array, int tSize) {
    double sum = 0.0;
    for (int i = 0; i < tSize; i++) {
        sum += array[i];

    }
    sum = sum / (double) tSize;
    return sum;
}

double EDAUtils::Variance(double* array, int num) {
    double avg = Mean(array, num);
    double var = 0;
    for (int i = 0; i < num; i++) {
        var += ((array [i] - avg) * (array [i] - avg));
    }
    var /= (num - 1);
    return var;
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

