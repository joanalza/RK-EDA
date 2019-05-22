/*
 * File:   RKEDA.cpp
 * Author: mayowaayodele
 *
 * Created on 13 February 2017, 22:09
 */
 //#include "stdafx.h"
#include "DRKEDA.h"

#include "EDAUtils.h"
#include "Tools.h"
#include "PFSP.h"
#include "srandom.h"
#include "AziziAdaptativeCooling.h"
#include "LinearCooling.h"
//#include "random.h"

#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdio.h>      /* printf */
#include <math.h>       /* pow */

DRKEDA::DRKEDA(int popSize, std::string problemPath, std::string dynamicPath, int FEs, int truncSize, int elitism, std::string results, std::string model, unsigned long theseed, int restart){
	m_populationSize = popSize;
	m_fileName = problemPath;
	m_dynamicPath = dynamicPath;
	m_FEs = FEs;
	m_truncationSize = truncSize;
	m_elitism = elitism;
	m_resultsPath = results;
	m_seed = theseed;
	m_restart = restart;
	m_modelPath = model;

    m_problemSize = 0;
}


void DRKEDA::runAlgorithm(double minTemp, double heating) {

	// Initialization of the space
	RK *bestSolution, *previousBest, *bestSolutionOfPopulation, *bestPerChange, *child, *trackingQuality;
	double stdev = 0, r, mean;
	double muMin = 0, muMax = 0;
	bool changed = false, isfirst;
	double* childAct, *model;
	int maxGens = m_FEs / m_populationSize, noOfEvals = 0, improvement = 0;
	Random *random;
	clock_t  starttime = clock();
	vector<RK*> pop(m_populationSize);
//	vector<RK*> population(m_populationSize);
	int i, j, x;

	// Read static problem
	m_problemSize = m_dfsp.ReadTaillardInstance(m_fileName, m_dynamicPath);

	// Save required data to rename the output file
	string distance = m_dfsp.getDistanceType(m_dynamicPath);
	string distanceNumber = m_dfsp.getDistanceMagnitude(m_dynamicPath);
	string algorithm = "";
	if (m_restart == 0)
		algorithm = "d";
	else
		algorithm = "r";

	// Initialise required parameters
	if (m_seed == -1) {
		random = new Random(new SRandom(time(NULL)));
	}
	else {
		random = new Random(new SRandom(m_seed));
	}

	if(m_FEs == -1){
		m_FEs = 1000 * pow(m_problemSize,2); // Evaluations = 1000 * n^2
	}

	srand(m_seed);

//	cout << "gen\tfes\tbestFit\tavgFit\t\tbestFound\tnoCurrentImprov\tsd\tchange\tchangeGen\tbestPerChange" << endl;
//	string results1 = "FileName \t Solution \tFitness \t err \t FEs \n";
//	string results = "gen,fes,bestFit,avgFit,bestFound,noCurrentImprovementCounter,sd,change,changeGen,bestPerChange,run,version,distance,distNumber,AverageKeyRange\n";
	string results = "gen,bestFit,sd,change,changeGen,bestPerChange,AverageKeyRange,modelQuality\n";
//	string modelTracking = "gen;fitness;permutation\n";
	string landscapeInterpretation = "changePeriod;identity;permutation;fitness\n";

	double avgFitness;
	// Generate a random population (evaluate solutions and normalise)
	for (i = 0; i < m_populationSize; i++) {

		// Create the random keys of the individuals
		childAct = new double[m_problemSize];
		for (x = 0; x < m_problemSize; x++) {
			r = ((double)rand() / (RAND_MAX));
			childAct[x] = r;
		}
		child = new RK(childAct, m_problemSize);
//		cout << "RK : [" << child->getRandomKeyAsString() << "]" << endl;

		// Set permutation, normalise and
		child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
		child->normalise();
		child->setFitness(m_dfsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
//		cout << "RK (perm): [" << child->getPermutationAsString() << "]" << endl;
		noOfEvals++;
		pop[i] = (child);
		if (i == 1)
			bestSolution = pop[i];
		else
			bestSolution = m_e.minimumSolution(pop[i],bestSolution);
		avgFitness += pop[i]->getFitness();
	}

//	bestSolution = m_e.getBestSolutionMin(pop);
	previousBest = bestSolution->Clone2();
	bestSolutionOfPopulation = bestSolution->Clone2();
	bestPerChange = bestSolution->Clone2();
	avgFitness = avgFitness / m_populationSize;

//	avgFitness = m_e.getPopulationAverageFitness(pop);

	int gen = 0, ichange = 1, genChange = 1;

	// Initialise cooling scheme
	heating = maxGens * m_dfsp.getChangeStep(ichange);
	LinearCooling *cooling = new LinearCooling(minTemp, heating);


	// Iterative process
	do {
		// Check if a change has occurred and change it if so
//		bool detect = m_dfsp.detectChange(previousBest, bestSolutionOfPopulation);
		if(m_dfsp.EvaluateFSPTotalFlowtime(bestSolutionOfPopulation->getPermutation()) != bestSolutionOfPopulation->getFitness()){
//			cout << "REACTED" << ichange << endl;
			if (m_restart == 0) {
				for (i = 0; i < m_populationSize; i++) {
					pop.at(i)->fitness = m_dfsp.EvaluateFSPTotalFlowtime(
							pop.at(i)->permutation);
//						cout <<  "D " << pop.at(i)->getPermutationAsString() << endl;
				}
			} else {
				for (i = 0; i < m_populationSize; i++) {
					// Create the random keys of the individuals
					childAct = new double[m_problemSize];
					for (x = 0; x < m_problemSize; x++) {
						r = ((double) rand() / (RAND_MAX));
						childAct[x] = r;
					}
					child = new RK(childAct, m_problemSize);

					child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
					child->normalise();
					child->setFitness(m_dfsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
//					cout << "R " << child->getPermutationAsString() << endl;
					noOfEvals++;
					pop[i] = (child);
				}
			}
			cout << noOfEvals << "; " << stdev << "; " << bestSolution->getFitness() << "; [" << bestSolution->getPermutationAsString() << "]" << gen <<endl;
			bestSolutionOfPopulation = m_e.getBestSolutionMin(pop);
			avgFitness = m_e.getPopulationAverageFitness(pop);
			bestPerChange = m_e.getBestSolutionMin(pop);

			if (ichange > m_dfsp.m_changes)
				heating = maxGens - (maxGens * m_dfsp.getChangeStep(ichange - 1));
			else
				heating = (maxGens * m_dfsp.getChangeStep(ichange)) - (maxGens * m_dfsp.getChangeStep(ichange - 1));
			cooling = new LinearCooling(minTemp, heating);
		}

		stdev = cooling->getNewTemperature(genChange);

		// Learn model
		model = new double[m_problemSize];
		model = m_e.getPM(pop, m_truncationSize, m_problemSize);
//		for (int i = 0; i< m_problemSize; i++){
//					cout << "v.1" << matrix[i] << endl;
//				}
		trackingQuality = new RK(model, m_problemSize);
		trackingQuality->setPermutation(m_e.randomKeyToAL(model, m_problemSize));
		trackingQuality->setFitness(m_dfsp.EvaluateFSPTotalFlowtime(trackingQuality->getPermutation()));

		muMin = m_e.lowest(model, m_problemSize);
		muMax = m_e.highest(model, m_problemSize);

//		cout << (gen+1) << "\t" << noOfEvals << "\t" << bestSolutionOfPopulation->getFitness() << "\t"
//				<< avgFitness << "\t" << bestSolution->getFitness() << "\t\t" << improvement << "\t" << stdev << "\t" << ichange
//				<< "\t" << genChange << "\t" << bestChange->getFitness() << endl;
		results += to_string(static_cast<long long>(gen+1)) + "," +
					std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "," +
					std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) + "," +
					to_string(static_cast<long long>(genChange)) + "," +
					std::to_string(static_cast<long long>(bestPerChange->getFitness())) + "," +
					std::to_string(static_cast<long double>(muMax - muMin)) + "," +
					std::to_string(static_cast<long double>(trackingQuality->getFitness())) +  "\n";

		previousBest =  bestSolutionOfPopulation->Clone();

		// Sample model
		isfirst = true;
		for (j = 0; j < m_populationSize; j++) {
			if (isfirst && m_elitism) {
				child = bestSolutionOfPopulation->Clone2();
				isfirst = false;
			}else{
				childAct = new double[m_problemSize];
				for (x = 0; x < m_problemSize; x++) {
					mean = model[x];
					childAct[x] = random->normal(stdev) + mean;
				}
				child = new RK(childAct, m_problemSize);
				child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
				child->normalise();
				child->setFitness(m_dfsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
				noOfEvals++;
			}
			pop[j] = child;
//			population[j] = child;
			// OVERALL
			if (bestSolution->getFitness() > child->getFitness()) {
				bestSolution = child->Clone2();

//				results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(noOfEvals) + "\n"; // ##C++11
//				results1 = m_fileName + "\t" + m_e.perm2str(bestSolution->getPermutation(), m_problemSize) + "\t" + std::to_string(static_cast<long double>(bestSolution->getFitness())) + "\t" + "0" + "\t" + to_string(static_cast<long long>(noOfEvals)) + "\n"; // ##C++0x
			}
		}

//		bestSolutionOfPopulation = m_e.getBestSolutionMin(population);
		bestSolutionOfPopulation = m_e.getBestSolutionMin(pop);

		// CURRENT BEST
//		if (previousBest->getFitness() > bestSolutionOfPopulation->getFitness()){
//			improvement = 0;
//		}else{
//			improvement++;
//		}

		// BEST PER CHANGE
		if (bestPerChange->getFitness() > bestSolutionOfPopulation->getFitness()){
			bestPerChange =  bestSolutionOfPopulation->Clone2();
		}

//		pop = population;
		avgFitness = m_e.getPopulationAverageFitness(pop);
		gen++;
		genChange++;

		delete[] model;

		clock_t  endtime = clock();

		changed = m_dfsp.changeIdentityPermutation(gen, maxGens);
		if(changed){
			landscapeInterpretation += to_string(static_cast<long long>(ichange)) + ";" + m_e.perm2str(m_dfsp.getIdentityPermutation(ichange - 1), m_problemSize) + ";" +
//					to_string(static_cast<long long>(bestSolutionOfPopulation->getPermutation()[0] + 1));
//			for (i = 1; i< m_problemSize; i++)
//				landscapeInterpretation += "," + to_string(static_cast<long long>(bestSolutionOfPopulation->getPermutation()[i] + 1)) ;
					m_e.perm2str(bestSolutionOfPopulation->getPermutation(), m_problemSize) +
					std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "\n";
			genChange = 1;
			ichange++;
//			cout << landscapeInterpretation << endl;
			cout <<  "------------------------" << ichange << "----------------------" << gen << endl;

		}

//		cout << (((double) (endtime - starttime)) / CLOCKS_PER_SEC) << endl;
	} while(gen < maxGens);


//	cout << (gen+1) << "\t" << noOfEvals << "\t" << bestSolutionOfPopulation->getFitness() << "\t"
//			<< avgFitness << "\t" << bestSolution->getFitness() << "\t\t" << improvement << "\t" << stdev << "\t" << ichange
//			<< "\t" << genChange << "\t" << bestChange->getFitness() << endl;
	results += to_string(static_cast<long long>(gen+1)) + "," +
				std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "," +
				std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) + "," +
				to_string(static_cast<long long>(genChange)) + "," +
				std::to_string(static_cast<long long>(bestPerChange->getFitness())) + "," +
				std::to_string(static_cast<long double>(muMax - muMin)) +"\n";

	for (i = 0; i < pop.size(); i++){
		delete pop[i];
	}


	ofstream myfile1;
	myfile1.open(m_resultsPath);
	myfile1 << results;
	myfile1.close();

	ofstream myfile2;
	myfile2.open(m_modelPath);
	myfile2 << landscapeInterpretation;
	myfile2.close();

}
