/* 
 * File:   EDAUtils.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:37
 */

#ifndef EDAUTILS_H
#define	EDAUTILS_H
#include <vector>       // std::vector
#include <random>       // std::vector
using namespace std;
#include "PFSP.h"
#include "RK.h"
class EDAUtils {
public:
     EDAUtils();
    virtual ~EDAUtils();
    bool myfunction(RK* i, RK* j);
    struct byFitness;
    struct byPermutation;
    vector<RK*> sortPopulation(vector<RK*> pop);
    void sortPop(vector<RK*> pop);
    bool isSortedPopulation(vector<RK*> pop);
    bool areRepeatedInPopulation(vector<RK*> pop);
    RK* getBestSolutionMin(vector<RK*> pop);
    bool popContainsPermutation(vector<RK*> pop, RK *x);
    double getPopulationAverageFitness(vector<RK*> pop);
//    RK* getBestSolutionMax(vector<RK*> pop);
    void getPM(vector<RK*> currentPopulation, int truncSize, int pSize, double* model);
    double* getChild(double* currentModel, double stdev, int pSize);
//    vector<double*> getPopulation(double* currentModel, double stdev, int probSize, int populationSize);
//	vector<RK*> initialPopulationRK(int probSize, int populationSize, string fileName);
//    vector<RK*> getPopulationRK(double* currentModel, double stdev, int probSize, int populationSize);
    double Mean(double* array, int tSize);
	//double* EDAUtils::getChild(double* currentModel, double stdev, int m_pSize) {
	//    //        double[] currentModel = Arrays.copyOf(currentModel1,currentModel1.length );
	//    double* childAct = new double[m_pSize];
	//    //std::default_random_engine generator; // ##C++11
	//	std::minstd_rand generator; // ##C++0x
	//
	//    for (int x = 0; x < m_pSize; x++) {
	//        //             double r2 = r.nextGaussian() * currentModel[x][1] + currentModel[x][0];
	//        double mean = currentModel[x];
	//
	//        std::normal_distribution<double> distribution(mean, stdev);
	//        double r2 = distribution(generator);
	////        cout << r2 << endl;
	//        childAct[x] = r2;
	//
	//    }
	//    return childAct;
	//}
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
	//int* EDAUtils::randomKeyToAL(double* rks, int problemSize) {
	//
	//
	//	double* aux_rks = new double[problemSize];
	//    vector<int> AL;
	//
	//    for (int i = 0; i < problemSize; i++) {
	//		aux_rks[i] = rks[i];
	//    }
	//    Tools::printRK(aux_rks, problemSize);
	//	sort(aux_rks, aux_rks + problemSize);
	//	Tools::printRK(aux_rks, problemSize);
	//	int k = 0;
	//
	//	for (int i = 0; i < problemSize; i++) {
	//		for (int j = 0; j < problemSize; j++) {
	//            if (((aux_rks[i] == rks[j]) && AL.empty() == true) || ((aux_rks[i] == rks[j]) && ((std::find(AL.begin(), AL.end(), j) != AL.end()) == false))) {
	//
	//                AL.push_back(j);
	//				k++;
	//            }
	//
	//            if (k == problemSize) {
	//                goto outer;
	//            }
	//
	//        }
	//
	//    }
	//
	//outer:
	//
	//    // cout << perm2str(AL, m_pSize) << endl;
	//	int* AL1 = new int[AL.size()];
	////	int* AL2 = new int[AL.size()];
	//    for (unsigned int i = 0; i < AL.size(); i++) {
	//        AL1[i] = AL[i];
	//    }
	////    Tools::Invert(AL1, m_pSize, AL2);
	//    //Tools::PrintArray(AL2, m_pSize, "Perm: ");
	//    AL.clear();
	//    //AL.shrink_to_fit(); // ##C++11
	//	AL.swap(AL); // ##C++0x
	//    delete[] aux_rks;
	//    return AL1;
	//}

//    int* randomKeyToAL(double rks[], int problemSize);
    RK* minimumSolution(RK* perm1, RK* perm2);
    RK* maximumSolution(RK* perm1, RK* perm2);

    double Variance(double* array, int num);
    double lowest(double* array, int size);
    double highest(double* array, int size);
    string perm2str(int* p, int n);

private:

};


#endif	/* EDAUTILS_H */

