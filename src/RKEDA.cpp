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
#include "LinearCooling.h"
#include "DynPermP.h"
//#include "random.h"

#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdio.h>      /* printf */
#include <math.h>       /* pow */

RKEDA::RKEDA(){ }
RKEDA::RKEDA(int popSize, string problemPath, string dynamicPath, int generations, int truncSize, int elitism, string results, unsigned long theseed, int restart){
	m_populationSize = popSize;
	m_fileName = problemPath;
	m_dynamicPath = dynamicPath;
	m_generations = generations;
	m_truncationSize = truncSize;
	m_elitism = elitism;
	m_resultsPath = results;
	m_seed = theseed;
	m_restart = restart;

    m_problemSize = 0;
}

RKEDA::~RKEDA() {

}

void RKEDA::runAlgorithm(double minTemp, double heating) {

	// Initialization of the space
	double stdev = 0, r, mean;
	double muMin = 0, muMax = 0, avgPopFitness = 0;
	bool changed = false, isfirst;
	int numOfEvals = 0, improvement = 0;
	int i, j, x;

	double *childAct, *model;
	Random *random;
	RK *bestSolution, *previousBest, *bestSolutionOfPopulation, *bestPerChange;
	vector<RK*> pop(m_populationSize); //+2
	

	// Read static problem
	m_dop = new DPFSP(); //+3
	m_problemSize = m_dop->ReadInstance(m_fileName); // +13 (+6)
	m_dop->ReadDynamic(m_dynamicPath); //+12 (+1)

	// Initialise required parameters
	if (m_seed == -1) {
		random = new Random(new SRandom(time(NULL)));
	}
	else {
		random = new Random(new SRandom(m_seed)); //+2
	}
	srand(m_seed);

	if(m_generations == -1){
		m_generations = 1000 * pow(m_problemSize,2); // Evaluations = 1000 * n^2
	}
	
	model = new double[m_problemSize];
	childAct = new double[m_problemSize];

	string results = "gen,popAverage,sd,change,changeGen,bestPerChange,AverageKeyRange\n"; //+2

	// Initialise cooling scheme
	int gensPerChange = m_generations / (m_dop->getNumOfChanges() + 1);
	LinearCooling *cooling = new LinearCooling(minTemp, heating, gensPerChange); //+1

	// Generate a random population (evaluate solutions and normalise)
	avgPopFitness = 0;
	for (i = 0; i < m_populationSize; i++) { //+3*popSize
		// Create the random keys of the individuals
		for (x = 0; x < m_problemSize; x++) {
			r = ((double)rand() / (RAND_MAX));
			childAct[x] = r;
		}
		// Generate and save in the population
		pop[i] = new RK(childAct, m_problemSize); //+3
		pop[i]->normalise();
		pop[i]->setFitness(m_dop->Evaluate(pop[i]->getPermutation()));
		numOfEvals++;
		if (i == 0)
			bestSolution = pop[i];
		else {
			if (bestSolution->getFitness() > pop[i]->getFitness())
				bestSolution = pop[i];
		}
		avgPopFitness += pop.at(i)->getFitness();
	}
//	bestSolution = m_e.getBestSolutionMin(pop);
	previousBest = bestSolution->Clone(); //+3
	bestSolutionOfPopulation = bestSolution->Clone(); //+3
	bestPerChange = bestSolution->Clone(); //+3
	avgPopFitness = avgPopFitness / m_populationSize;

	int gen = 0, ichange = 1, genChange = 1;

	// Iterative process
	do {
		pop = m_e.sortPopulation(pop);
		// Check if a change has occurred and change it if so
		if(m_dop->Evaluate(bestSolutionOfPopulation->getPermutation()) != bestSolutionOfPopulation->getFitness()){
			avgPopFitness = 0;
			if (m_restart == 0) {
				for (i = 0; i < m_populationSize; i++) {
					pop.at(i)->setFitness(m_dop->Evaluate(pop.at(i)->m_permutation));
					avgPopFitness += pop.at(i)->getFitness();
				}
			} else {
				for (i = 0; i < m_populationSize; i++) {
					for (x = 0; x < m_problemSize; x++) {
						r = ((double)rand() / (RAND_MAX));
						childAct[x] = r;
					}
					pop[i]->setRandomKeys(childAct);
					pop[i]->setPermFromRKs();
					pop[i]->normalise();
					pop[i]->setFitness(m_dop->Evaluate(pop[i]->getPermutation()));
					numOfEvals++;
					avgPopFitness += pop.at(i)->getFitness();
				}
			}
			pop = m_e.sortPopulation(pop);
					
			bestSolutionOfPopulation->deepCopy(pop[0]);
			avgPopFitness = avgPopFitness / m_populationSize;
			bestPerChange->deepCopy(pop[0]);

			//cout << "Avg: "<< avgPopFitness << endl;
			//cout << "New situation: from" << Tools::perm2str(bestSolutionOfPopulation->m_permutation, m_problemSize) << ", " << bestSolutionOfPopulation->getFitness() << endl;
			//cout << "ALL best: " << bestSolution->getFitness() << "; Current best: [" << Tools::perm2str(bestPerChange->m_permutation, m_problemSize) << "], " << bestPerChange->m_fitness << endl;
//			if (ichange > m_dop->m_changes)
//				heating = m_generations - (m_generations * m_dop->getChangeStep(ichange - 1));
//			else
//				heating = (m_generations * m_dop->getChangeStep(ichange)) - (m_generations * m_dop->getChangeStep(ichange - 1));
			cooling->initialise(minTemp, heating, gensPerChange);
		}

		// Print population
		/*for (int i = 0; i<m_populationSize; i++)
			cout << Tools::rk2str(pop[i]->getNormalisedRKs(), pop[i]->getProblemSize()) << " " << pop[i]->getFitness() << endl;
		cout << avgPopFitness << endl;*/

		stdev = cooling->getNewTemperature(genChange);    // Linear Cooling
//		stdev = cooling->getNewTemperature(improvement);  // Adaptive cooling
//		cout << "Variance: " << stdev << endl;


		// Learn model
		m_e.getPM(pop, m_truncationSize, m_problemSize, model);

		// Range of the mode
		muMin = m_e.lowest(model, m_problemSize);
		muMax = m_e.highest(model, m_problemSize);

		// Save generation information
		results += to_string(static_cast<long long>(gen)) + "," +
					std::to_string(static_cast<long double>(avgPopFitness)) + "," +
					std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) + "," +
					to_string(static_cast<long long>(genChange)) + "," +
					std::to_string(static_cast<long long>(bestPerChange->getFitness())) + "," +
					std::to_string(static_cast<long double>(muMax - muMin)) +
					"\n";

		// Store previous generation information
		previousBest->deepCopy(bestSolutionOfPopulation);

		// Sample model
//		pop.clear();
		isfirst = true;
		avgPopFitness = 0;
		for (j = 0; j < m_populationSize; j++) {
			if (isfirst && m_elitism) {
				pop[j]->deepCopy(bestSolutionOfPopulation);
				isfirst = false;
			}else{
				//childAct = new double[m_problemSize];
				for (x = 0; x < m_problemSize; x++) {
					mean = model[x];
					childAct[x] = random->normal(stdev) + mean;
				}
				pop[j]->setRandomKeys(childAct);
				pop[j]->setPermFromRKs();
				pop[j]->normalise();
				pop[j]->setFitness(m_dop->Evaluate(pop[j]->getPermutation()));
				numOfEvals++;
			}
			avgPopFitness += pop.at(j)->getFitness();
			
			if (i == 0)
				bestSolutionOfPopulation = pop[j];
			else
				if (bestSolutionOfPopulation->getFitness() > pop[j]->getFitness())
					bestSolutionOfPopulation->deepCopy(pop[j]);
			
			// OVERALL
			if (bestSolution->getFitness() > pop[j]->getFitness()) {
				bestSolution->deepCopy(pop[j]);
			}
		}

		avgPopFitness = avgPopFitness / m_populationSize;

//		// CURRENT BEST
//		if (previousBest->getFitness() > bestSolutionOfPopulation->getFitness()){
//			improvement = 0;
//		}else{
//			improvement++;
//		}

		// BEST PER CHANGE
		if (bestPerChange->getFitness() > bestSolutionOfPopulation->getFitness()){
			bestPerChange->deepCopy(bestSolutionOfPopulation);
		}

		gen++;
		genChange++;


		changed = m_dop->changeIdentityPermutation(gen, m_generations);
		if(changed){
			genChange = 1;
			ichange++;
			cout << Tools::perm2str(bestSolutionOfPopulation->m_permutation, m_problemSize) << ". Last fit: " << bestSolutionOfPopulation->getFitness() << ". New Fit: " << m_dop->Evaluate(bestSolutionOfPopulation->getPermutation()) << endl;
//			cout << "Gen: " << gen << ". Change: " << ichange - 1<< ". Best: "<< bestPerChange->getFitness() << endl;

		}
		else {
//			cout << "Gen: " << gen << ". Change: " << ichange - 1 << ". Best: " << bestPerChange->getFitness() << ". Avg: " << avgPopFitness << endl;
		}

//		cout << (((double) (endtime - starttime)) / CLOCKS_PER_SEC) << endl;
	} while(gen < m_generations);


	results += to_string(static_cast<long long>(gen)) + "," +
				std::to_string(static_cast<long double>(avgPopFitness)) + "," +
				std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) + "," +
				to_string(static_cast<long long>(genChange)) + "," +
				std::to_string(static_cast<long long>(bestPerChange->getFitness())) + "," +
				std::to_string(static_cast<long double>(muMax - muMin)) +"\n";

	// Free up memory space
	delete[]model;
	delete[]childAct;
	delete cooling;
	for (i = 0; i < pop.size(); i++){
		delete pop[i];
	}
	pop.clear();
	delete m_dop;


	ofstream myfile1;
	myfile1.open(m_resultsPath);
	myfile1 << results;
	myfile1.close();

}
