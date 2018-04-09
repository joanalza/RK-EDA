/*
 * File:   RKEDA.cpp
 * Author: mayowaayodele
 *
 * Created on 13 February 2017, 22:09
 */
 //#include "stdafx.h"
#include "RKEDA.h"
#include "EDAUtils.h"
#include "Tools.h"
#include "PFSP.h"
#include "srandom.h"
#include "AziziAdaptativeCooling.h"
//#include "random.h"

#include <ctime>
#include <string>
#include <iostream>
#include <stdio.h>      /* printf */
#include <math.h>       /* pow */

RKEDA:: RKEDA(int popSize, std::string problemPath, std::string dynamicPath, int FEs, int truncSize, int elitism, std::string results, unsigned long theseed){
	m_populationSize = popSize;
	m_fileName = problemPath;
	m_dynamicPath = dynamicPath;
	m_FEs = FEs;
	m_truncationSize = truncSize;
	m_elitism = elitism;
	m_resultsPath = results;
	m_seed = theseed;
	m_variance = 0.05;
}

void RKEDA::runAlgorithm(double minTemp, double heating) {

	// Initialization of the space
	string results = "FileName \t Solution \tFitness \t err \t FEs \n";
	string results1 = "FileName \t Solution \tFitness \t err \t FEs \n";

	using std::cout;
	using std::endl;

	double stdev = 0;


	RK *bestSolution, *previousBest, *bestSolutionOfPopulation;
	m_fsp.ReadInstance(m_fileName);
	m_problemSize = m_fsp.JOB_NUM;

	vector<RK*> pop(m_populationSize);
	vector<RK*> population(m_populationSize);

	double r, mean;

	Random *random;

	if (m_seed == -1) {
		random = new Random(new SRandom(time(NULL)));
	}
	else {
		random = new Random(new SRandom(m_seed));
	}

	if(m_FEs == -1){
		m_FEs = 1000 * pow(m_problemSize,2); // Evaluations = 1000 * n^2
	}

	int noOfEvals = 0;
	int numberofGenerations = (m_FEs / m_populationSize);

	int improvement = 0;

	AziziAdaptativeCooling* cooling = new AziziAdaptativeCooling(minTemp, heating);

	time_t  starttime = time(0);
//	cout << "starttime:" << starttime << endl;
//	std::cout << "starttime:" << starttime << std::flush << endl;

	srand(m_seed);

	cout << "fes; gen; stdev; best" << endl;

	// Initialize the first population
	for (int i = 0; i < m_populationSize; i++) {
		//cout << "i: " << i << endl;

		// Create the random keys of the individuals
		double* childAct = new double[m_problemSize];
		for (int x = 0; x < m_problemSize; x++) {
			r = ((double)rand() / (RAND_MAX));
			childAct[x] = r;
		}
		RK* child = new RK(childAct, m_problemSize);

		child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
		child->normalise();
		child->setFitness(m_fsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
		noOfEvals++;
		pop[i] = (child);

		if (i == 0) {
			bestSolution = child;
		}
		else {
			if (bestSolution->getFitness() > child->getFitness()) {
				bestSolution = child;
			}
		}
	}
	previousBest = bestSolution->Clone2();



	// Iterative process
	for (int i = 0; i < numberofGenerations; i++) {
		if (noOfEvals >= m_FEs) {
			break;
		}
		stdev = cooling->getNewTemperature(improvement);

		double* matrix = new double[m_problemSize];
		matrix = m_e.getPM(pop, m_truncationSize, m_problemSize);

		for (int j = 0; j < m_populationSize; j++) {

			double* childAct = new double[m_problemSize];
			for (int x = 0; x < m_problemSize; x++) {
				// double r2 = r.nextGaussian() * currentModel[x][1] + currentModel[x][0];
				mean = matrix[x];
				childAct[x] = random->normal(stdev) + mean;
			}
			RK* child = new RK(childAct, m_problemSize);
			//            RK child(childAct, problemSize);
			child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
			child->normalise();
			child->setFitness(m_fsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
			noOfEvals++;

			if (noOfEvals >= m_FEs) {
				break;
			}
			population[j] = child;
			// OVERALL
			if (bestSolution->getFitness() > child->getFitness()) {
				bestSolution = child->Clone2();

				cout << noOfEvals << "; " << (i+1) << "; " << stdev << "; " << bestSolution->getFitness() << "; [" << bestSolution->getPermutationAsString() << "]" << endl;

				//results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(noOfEvals) + "\n"; // ##C++11
				results1 = m_fileName + "\t" + m_e.perm2str(bestSolution->getPermutation(), m_problemSize) + "\t" + std::to_string(static_cast<long double>(bestSolution->getFitness())) + "\t" + "0" + "\t" + to_string(static_cast<long long>(noOfEvals)) + "\n"; // ##C++0x
			}

			if (j == 0) {
				bestSolutionOfPopulation = child;
			}
			else {
				if (bestSolutionOfPopulation->getFitness() > child->getFitness()) {
					bestSolutionOfPopulation = child;
				}
			}
		}

		if (previousBest->getFitness() > bestSolutionOfPopulation->getFitness()){
			improvement = 0;
		}else{
			improvement++;
		}


		for (int i = 0; i < pop.size(); i++)
		{
			delete pop[i];
		}
		pop = population;

		delete[] matrix;
		//        population.clear();
	}

	time_t  endtime = time(0);

	ofstream myfile1;
	myfile1.open(m_resultsPath);

	myfile1 << results1;
	myfile1.close();

}
