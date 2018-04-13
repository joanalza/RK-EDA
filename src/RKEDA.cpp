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
#include <iomanip>
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

    m_problemSize = 0;
}

 void RKEDA::setResultsPath(std::string fileName, std::string dynamicName, double minTemp, double heating, int run){
	 // Create an output string stream
	 std::ostringstream streamObj1;
	 std::ostringstream streamObj2;
	 std::ostringstream streamObj3;

	 // Set Fixed -Point Notation
	 streamObj1 << std::fixed;
	 streamObj2 << std::fixed;
	 streamObj3 << std::fixed;

	 // Set precision to 2 digits
	 streamObj1 << std::setprecision(2);
	 streamObj2 << std::setprecision(2);
	 streamObj3 << std::setprecision(0);

	 //Add double to stream
	 streamObj1 << minTemp;
	 streamObj2 << heating;
	 streamObj3 << m_elitism;

	 // Get string from output string stream
	 std::string strObj1 = streamObj1.str();
	 std::string strObj2 = streamObj2.str();
	 std::string strObj3 = streamObj3.str();

	 size_t lastindex = fileName.find_last_of(".");
	 string instance = fileName.substr(0, lastindex);

	 lastindex = dynamicName.find_last_of(".");
	 string dynamic= dynamicName.substr(0, lastindex);

	 m_resultsPath += "process-" + instance+ "-" + dynamic + "-currentbest_" + strObj1 + "_" + strObj2 +
			 "-elt" + strObj3 + "--" + std::to_string(run) +".csv";
}


void RKEDA::runAlgorithm(double minTemp, double heating) {

	// Initialization of the space



	RK *bestSolution, *previousBest, *bestSolutionOfPopulation, *bestChange;
	//m_problemSize = m_fsp.ReadInstance(m_fileName);
	m_problemSize = m_dfsp.ReadTaillardInstance(m_fileName, m_dynamicPath);
	//m_dfsp.setIdentityPermutationChanges();

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

	int improvement = 0;

	AziziAdaptativeCooling *cooling = new AziziAdaptativeCooling(minTemp, heating);
	double stdev = cooling->getNewTemperature(improvement);
	time_t  starttime = time(0);

	srand(m_seed);

	cout << "gen\tfes\tbestFit\tavgFit\t\tbestFound\tnoCurrentImprov\tsd\tchange\tchangeGen\tbestPerChange" << endl;


	//string results1 = "FileName \t Solution \tFitness \t err \t FEs \n";
	string results = "gen,fes,bestFit,avgFit,bestFound,noCurrentImprovementCounter,sd,change,changeGen,bestPerChange\n";


	// Initialize the first population
	for (int i = 0; i < m_populationSize; i++) {

		// Create the random keys of the individuals
		double* childAct = new double[m_problemSize];
		for (int x = 0; x < m_problemSize; x++) {
			r = ((double)rand() / (RAND_MAX));
			childAct[x] = r;
		}
		RK* child = new RK(childAct, m_problemSize);

		child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
		child->normalise();
		child->setFitness(m_dfsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
		noOfEvals++;
		pop[i] = (child);
	}

	bestSolution = m_e.getBestSolutionMin(pop);
	previousBest = bestSolution->Clone2();
	bestSolutionOfPopulation = bestSolution->Clone2();
	bestChange = bestSolution->Clone2();

	double avgFitness = m_e.getPopulationAverageFitness(pop);

	int gen = 0, ichange = 1, genChange = 1;


	// Iterative process
	do/*for (int i = 0; i < numberofGenerations; i++)*/ {

		// Check if a change has occurred and change it if so
		bool changed = m_dfsp.changeIdentityPermutation(noOfEvals, m_FEs);
		if(changed){
			genChange = 1;
			for(int i=0; i< m_populationSize; i++){
				pop.at(i)->fitness = m_dfsp.EvaluateFSPTotalFlowtime(pop.at(i)->permutation);
			}
			bestSolutionOfPopulation = m_e.getBestSolutionMin(pop);
			avgFitness = m_e.getPopulationAverageFitness(pop);
			bestChange = m_e.getBestSolutionMin(pop);
			ichange++;
		}

		cout << (gen+1) << "\t" << noOfEvals << "\t" << bestSolutionOfPopulation->getFitness() << "\t"
				<< avgFitness << "\t" << bestSolution->getFitness() << "\t\t" << improvement << "\t" << stdev << "\t" << ichange
				<< "\t" << genChange << "\t" << bestChange->getFitness() << endl;
		results += to_string(static_cast<long long>(gen+1)) + "," + to_string(static_cast<long long>(noOfEvals)) + "," +
					std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "," +
					std::to_string(static_cast<long double>(avgFitness)) + "," +
					std::to_string(static_cast<long double>(bestSolution->getFitness())) + "," +
					to_string(static_cast<long long>(improvement)) + "," +
					std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) +
					"," + to_string(static_cast<long long>(genChange)) + "," +
					std::to_string(static_cast<long double>(bestChange->getFitness())) +"\n";


//		results1 = m_fileName + "\t" + m_e.perm2str(bestSolution->getPermutation(), m_problemSize) + "\t" + std::to_string(static_cast<long double>(bestSolution->getFitness())) + "\t" + "0" + "\t" + to_string(static_cast<long long>(noOfEvals)) + "\n"; // ##C++0x

		double* matrix = new double[m_problemSize];
		matrix = m_e.getPM(pop, m_truncationSize, m_problemSize);

		for (int j = 0; j < m_populationSize; j++) {

			double* childAct = new double[m_problemSize];
			for (int x = 0; x < m_problemSize; x++) {
				mean = matrix[x];
				childAct[x] = random->normal(stdev) + mean;
			}
			RK* child = new RK(childAct, m_problemSize);
			child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
			child->normalise();
			child->setFitness(m_dfsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
			noOfEvals++;

			if (noOfEvals >= m_FEs) {
				break;
			}
			population[j] = child;
			// OVERALL
			if (bestSolution->getFitness() > child->getFitness()) {
				bestSolution = child->Clone2();

//				cout << noOfEvals << "; " << (i+1) << "; " << stdev << "; " << bestSolution->getFitness() << "; [" << bestSolution->getPermutationAsString() << "]" << endl;

//				results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(noOfEvals) + "\n"; // ##C++11
//				results1 = m_fileName + "\t" + m_e.perm2str(bestSolution->getPermutation(), m_problemSize) + "\t" + std::to_string(static_cast<long double>(bestSolution->getFitness())) + "\t" + "0" + "\t" + to_string(static_cast<long long>(noOfEvals)) + "\n"; // ##C++0x
			}
		}

		bestSolutionOfPopulation = m_e.getBestSolutionMin(population);

		// CURRENT BEST
		if (previousBest->getFitness() > bestSolutionOfPopulation->getFitness()){
			improvement = 0;
		}else{
			improvement++;
		}
		previousBest =  bestSolutionOfPopulation->Clone2();

		// NEST PER CHANGE
		if (bestChange->getFitness() > bestSolutionOfPopulation->getFitness()){
			bestChange =  bestSolutionOfPopulation->Clone2();
		}



		for (int i = 0; i < pop.size(); i++){
			delete pop[i];
		}

		pop = population;
		avgFitness = m_e.getPopulationAverageFitness(pop);
		stdev = cooling->getNewTemperature(improvement);
		gen++;
		genChange++;

		delete[] matrix;

	} while(noOfEvals < m_FEs);


	cout << (gen+1) << "\t" << noOfEvals << "\t" << bestSolutionOfPopulation->getFitness() << "\t"
			<< avgFitness << "\t" << bestSolution->getFitness() << "\t\t" << improvement << "\t" << stdev << "\t" << ichange
			<< "\t" << genChange << "\t" << bestChange->getFitness() << endl;
	results += to_string(static_cast<long long>(gen+1)) + "," + to_string(static_cast<long long>(noOfEvals)) + "," +
				std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "," +
				std::to_string(static_cast<long double>(avgFitness)) + "," +
				std::to_string(static_cast<long double>(bestSolution->getFitness())) + "," +
				to_string(static_cast<long long>(improvement)) + "," +
				std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) +
				"," + to_string(static_cast<long long>(genChange)) + "," +
				std::to_string(static_cast<long double>(bestChange->getFitness())) +"\n";


	time_t  endtime = time(0);

	ofstream myfile1;
	myfile1.open(m_resultsPath);
//	myfile1 << results1;
	myfile1 << results;
	myfile1.close();

}
