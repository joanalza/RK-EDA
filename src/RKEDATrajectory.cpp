///*
// * File:   RKEDATrajectory.cpp
// * Author: mayowaayodele
// *
// * Created on 13 February 2017, 22:09
// */
// //#include "stdafx.h"
//#include "RKEDATrajectory.h"
//
//#include "EDAUtils.h"
//#include "Tools.h"
//#include "PFSP.h"
//#include "srandom.h"
//#include "AziziAdaptativeCooling.h"
//#include "LinearCooling.h"
////#include "random.h"
//
//#include <ctime>
//#include <string>
//#include <iostream>
//#include <iomanip>
//#include <stdio.h>      /* printf */
//#include <math.h>       /* pow */
//
//RKEDATrajectory::RKEDATrajectory(int popSize, std::string problemPath, std::string dynamicPath, int generations, int truncSize, int elitism, std::string results, std::string model, unsigned long theseed, int restart, std::string modelPermutation){
//	m_populationSize = popSize;
//	m_fileName = problemPath;
//	m_dynamicPath = dynamicPath;
//	m_generations = generations;
//	m_truncationSize = truncSize;
//	m_elitism = elitism;
//	m_resultsPath = results;
//	m_seed = theseed;
//	m_restart = restart;
//	m_modelPath = model;
//	m_modelPermFile = modelPermutation;
//
//    m_problemSize = 0;
//}
//
//
//void RKEDATrajectory::runAlgorithm(double minTemp, double heating) {
//
//	// Initialization of the space
//	RK *bestSolution, *previousBest, *bestSolutionOfPopulation, *bestPerChange, *child, *mode;
//	double stdev = 0, r, mean;
//	double muMin = 0, muMax = 0;
//	bool changed = false, isfirst;
//	double *childAct, *model;
//	int noOfEvals = 0, improvement = 0;
//	Random *random;
//	clock_t  starttime = clock();
//	vector<RK*> pop(m_populationSize);
////	vector<RK*> population(m_populationSize);
//	int i, j, x;
//
//	// Read static problem
//	m_dop = new DPFSP();
//	m_problemSize = m_dop->ReadInstance(m_fileName);
//	m_dop->ReadDynamic(m_dynamicPath);
//
//
//	// Save required data to rename the output file
//	string distance = m_dop->getDistanceType(m_dynamicPath);
//	string distanceNumber = m_dop->getDistanceMagnitude(m_dynamicPath);
//
////	cout << m_problemSize << "," << distance << endl;
//	string algorithm = "";
//	if (m_restart == 0)
//		algorithm = "d";
//	else
//		algorithm = "r";
//
//	// Initialise required parameters
//	if (m_seed == -1) {
//		random = new Random(new SRandom(time(NULL)));
//	}
//	else {
//		random = new Random(new SRandom(m_seed));
//	}
//
//	if(m_generations == -1){
//		m_generations = 1000 * pow(m_problemSize,2); // Evaluations = 1000 * n^2
//	}
//
//	srand(m_seed);
//
//	// Save external parameters
//	int* optimum;
//	optimum = new int[m_problemSize];
//	readModelPermutation(m_modelPermFile);
//	try{
//	optimum = m_modePermutations[0];
//	}catch (exception& e){
//		cout << "Model file not founded " << endl;
//	}
//	RK *optima = new RK();
//	optima->setProblemSize(m_problemSize);
//	optima->setPermutation(optimum);
//	cout << "Global optimum fines: " << Tools::perm2str(optimum, m_problemSize) << ": " << m_dop->Evaluate(optima->getPermutation()) << endl;
//	optima->setRKfromPermutation(optima->getPermutation());
////	optima->normaliseRanks(optimum);
//
////	cout << "Global optimum fines: " << Tools::rk2str(optima->m_randomkeys,m_problemSize) << ": " << m_dop->Evaluate(optima->getPermutation()) << endl;
//
////	cout << "gen\tfes\tbestFit\tavgFit\t\tbestFound\tnoCurrentImprov\tsd\tchange\tchangeGen\tbestPerChange" << endl;
////	string results1 = "FileName \t Solution \tFitness \t err \t FEs \n";
////	string results = "gen,fes,bestFit,avgFit,bestFound,noCurrentImprovementCounter,sd,change,changeGen,bestPerChange,run,version,distance,distNumber,AverageKeyRange\n";
//	string results = "gen,bestFit,sd,change,changeGen,bestPerChange,AverageKeyRange,modelQuality\n";
////	string modelTracking = "gen;m_fitness;m_permutation\n";
////	string landscapeInterpretation = "changePeriod;identity;m_permutation;m_fitness\n";
//	string trajectory = "generation;modePermutation;modeFitness\n";
//
//	double avgFitness;
//	// Generate a random population (evaluate solutions and normalise)
//	for (i = 0; i < m_populationSize; i++) {
//
//		// Create the random keys of the individuals
//		childAct = new double[m_problemSize];
//		for (x = 0; x < m_problemSize; x++) {
//			r = ((double)rand() / (RAND_MAX));
//			childAct[x] = r;
//		}
//		child = new RK(childAct, m_problemSize);
//
//		// Set m_permutation, normalise and
//		child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
//		child->normalise();
//		child->setFitness(m_dop->Evaluate(child->getPermutation()));
//		noOfEvals++;
//		pop[i] = (child);
//		if (i == 0)
//			bestSolution = pop[i];
//		else
//			bestSolution = m_e.minimumSolution(pop[i],bestSolution);
//		avgFitness += pop[i]->getFitness();
////		Tools::PrintArray(pop[i]->getPermutation(), pop[i]->m_pSize, "");
//	}
//
////	bestSolution = m_e.getBestSolutionMin(pop);
//	previousBest = bestSolution->Clone();
//	bestSolutionOfPopulation = bestSolution->Clone();
//	bestPerChange = bestSolution->Clone();
//	avgFitness = avgFitness / m_populationSize;
//
////	avgFitness = m_e.getPopulationAverageFitness(pop);
//
//	int gen = 0, ichange = 1, genChange = 1;
//	// Initialise cooling scheme
////	heating = m_generations ; //* m_dop->getChangeStep(ichange);
////	cout << m_dop->getNumOfChanges() << endl;
//	int gensPerChange = m_generations / (m_dop->getNumOfChanges() + 1);
////
//	LinearCooling *cooling = new LinearCooling(minTemp, heating, gensPerChange);
//
//	// Iterative process
//	do {
//		// Check if a change has occurred and change it if so
////		bool detect = m_dfsp.detectChange(previousBest, bestSolutionOfPopulation);
//		if(m_dop->Evaluate(bestSolutionOfPopulation->getPermutation()) != bestSolutionOfPopulation->getFitness()){
//			cout << "REACTED" << ichange << endl;
//			if (m_restart == 0) {
//				for (i = 0; i < m_populationSize; i++) {
//					pop.at(i)->m_fitness = m_dop->Evaluate(pop.at(i)->m_permutation);
////						cout <<  "D " << pop.at(i)->getPermutationAsString() << endl;
//				}
//			} else {
//				for (i = 0; i < m_populationSize; i++) {
//					// Create the random keys of the individuals
//					childAct = new double[m_problemSize];
//					for (x = 0; x < m_problemSize; x++) {
//						r = ((double) rand() / (RAND_MAX));
//						childAct[x] = r;
//					}
//					child = new RK(childAct, m_problemSize);
//
//					child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
//					child->normalise();
//					child->setFitness(m_dop->Evaluate(child->getPermutation()));
////					cout << "R " << child->getPermutationAsString() << endl;
//					noOfEvals++;
//					pop[i] = (child);
//				}
//			}
////			cout << noOfEvals << "; " << stdev << "; " << bestSolution->getFitness() << "; [" << bestSolution->getPermutationAsString() << "]" << gen <<endl;
//			bestSolutionOfPopulation = m_e.getBestSolutionMin(pop);
//			avgFitness = m_e.getPopulationAverageFitness(pop);
//			bestPerChange = m_e.getBestSolutionMin(pop);
//
////			if (ichange > m_dop->m_changes)
////				heating = m_generations - (m_generations * m_dop->getChangeStep(ichange - 1));
////			else
////				heating = (m_generations * m_dop->getChangeStep(ichange)) - (m_generations * m_dop->getChangeStep(ichange - 1));
//			cooling = new LinearCooling(minTemp, heating, gensPerChange);
//		}
//
//		stdev = cooling->getNewTemperature(genChange);    // Linear Cooling
////		stdev = cooling->getNewTemperature(improvement);  // Adaptive cooling
////		cout << "Variance: " << stdev << endl;
//
//
//		// Learn model
//		model = new double[m_problemSize];
//		m_e.getPM(pop, m_truncationSize, m_problemSize, model);
//
//		// Trace the trajectory
////		Tools::printarray(model, m_problemSize);
////		if (gen == 0){
////			model = optima->m_randomkeys;
////			Tools::printarray(model, m_problemSize);
//////			double *copy = new double[m_problemSize];
//////			for (int i=0; i < m_problemSize; i++){
//////				copy[i] = model[optimum[i]];
//////			}
//////			model = ;
//////			Tools::printarray(model, m_problemSize);
////		}
//
//
//		mode = new RK(model, m_problemSize);
//		mode->setPermutation(m_e.randomKeyToAL(model, m_problemSize));
////		cout << "Mode: " << Tools::perm2str(mode->getPermutation(), m_problemSize) << ": " << m_dop->Evaluate(mode->getPermutation()) << endl;
//		mode->setFitness(m_dop->Evaluate(mode->getPermutation()));
//
//		muMin = m_e.lowest(model, m_problemSize);
//		muMax = m_e.highest(model, m_problemSize);
//
////		cout << "Model range: " << muMin << "-" << muMax << endl;
//
////		cout << (gen+1) << "\t" << noOfEvals << "\t" << bestSolutionOfPopulation->getFitness() << "\t"
////				<< avgFitness << "\t" << bestSolution->getFitness() << "\t\t" << improvement << "\t" << stdev << "\t" << ichange
////				<< "\t" << genChange << "\t" << bestChange->getFitness() << endl;
//		results += to_string(static_cast<long long>(gen+1)) + "," +
//					std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "," +
//					std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) + "," +
//					to_string(static_cast<long long>(genChange)) + "," +
//					std::to_string(static_cast<long long>(bestPerChange->getFitness())) + "," +
//					std::to_string(static_cast<long double>(muMax - muMin)) + "," +
//					std::to_string(static_cast<long double>(mode->getFitness())) +  "\n";
//
//		trajectory +=  to_string(static_cast<long long>(gen+1)) + ";" + m_e.perm2str(mode->getPermutation(), mode->m_pSize) + ";" +
//				to_string(static_cast<long double>(mode->getFitness())) +  "\n";
//
//
//		previousBest =  bestSolutionOfPopulation->Clone();
//
//		// Sample model
//		pop.clear();
//		isfirst = true;
//		for (j = 0; j < m_populationSize; j++) {
//			if (isfirst && m_elitism) {
//				child = bestSolutionOfPopulation->Clone();
//				isfirst = false;
//			}else{
//				childAct = new double[m_problemSize];
//								for (x = 0; x < m_problemSize; x++) {
//									mean = model[x];
//									childAct[x] = random->normal(stdev) + mean;
//								}
//								child = new RK(childAct, m_problemSize);
//								child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
//								child->normalise();
//								child->setFitness(m_dop->Evaluate(child->getPermutation()));
//								noOfEvals++;
////				int counter =0;
//////				do{
////					childAct = new double[m_problemSize];
////					for (x = 0; x < m_problemSize; x++) {
////						mean = model[x];
////						childAct[x] = random->normal(stdev) + mean;
////					}
////					child = new RK(childAct, m_problemSize);
////					child->setPermutation(m_e.randomKeyToAL(childAct, m_problemSize));
//////					counter++;
//////					cout << counter << endl;
//////				}while(m_e.popContainsPermutation(pop, child));
////				child->normalise();
////				child->setFitness(m_dop->Evaluate(child->getPermutation()));
////				noOfEvals++;
//			}
//			pop.push_back(child);
//			std::stringstream ss;
//			ss << "Sol" <<j << ": ";
//			string sol = ss.str();
////			Tools::PrintArray(pop[j]->getPermutation(), pop[j]->m_pSize, sol);
////			population[j] = child;
//			// OVERALL
//			if (bestSolution->getFitness() > child->getFitness()) {
//				bestSolution = child->Clone();
//
////				results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(noOfEvals) + "\n"; // ##C++11
////				results1 = m_fileName + "\t" + m_e.perm2str(bestSolution->getPermutation(), m_problemSize) + "\t" + std::to_string(static_cast<long double>(bestSolution->getFitness())) + "\t" + "0" + "\t" + to_string(static_cast<long long>(noOfEvals)) + "\n"; // ##C++0x
//			}
//		}
////		cout << pop.size() << endl;
//
////		bestSolutionOfPopulation = m_e.getBestSolutionMin(population);
//		bestSolutionOfPopulation = m_e.getBestSolutionMin(pop);
//
//		// CURRENT BEST
//		if (previousBest->getFitness() > bestSolutionOfPopulation->getFitness()){
//			improvement = 0;
//		}else{
//			improvement++;
//		}
//
//		// BEST PER CHANGE
//		if (bestPerChange->getFitness() > bestSolutionOfPopulation->getFitness()){
//			bestPerChange =  bestSolutionOfPopulation->Clone();
//		}
//
////		pop = population;
//		avgFitness = m_e.getPopulationAverageFitness(pop);
//		gen++;
//		genChange++;
//
//		delete []model;
////		delete []childAct;
//
//		clock_t  endtime = clock();
//
//		changed = m_dop->changeIdentityPermutation(gen, m_generations);
//		if(changed){
////			landscapeInterpretation += to_string(static_cast<long long>(ichange)) + ";" +
////					m_e.perm2str(m_dop->getIdentityPermutation(ichange - 1), m_problemSize) + ";" +
//////					to_string(static_cast<long long>(bestSolutionOfPopulation->getPermutation()[0] + 1));
//////			for (i = 1; i< m_problemSize; i++)
//////				landscapeInterpretation += "," + to_string(static_cast<long long>(bestSolutionOfPopulation->getPermutation()[i] + 1)) ;
////					m_e.perm2str(bestSolutionOfPopulation->getPermutation(), m_problemSize) +
////					std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "\n";
//			genChange = 1;
//			ichange++;
////			cout << landscapeInterpretation << endl;
//			cout <<  "------------------------" << ichange << "----------------------" << gen << endl;
//
//		}
//		cout << "Gen: " << gen << ". Change: " << ichange<< ". Varance: "<< stdev << endl;
//
////		cout << (((double) (endtime - starttime)) / CLOCKS_PER_SEC) << endl;
//	} while(gen < m_generations);
//
//
////	cout << (gen+1) << "\t" << noOfEvals << "\t" << bestSolutionOfPopulation->getFitness() << "\t"
////			<< avgFitness << "\t" << bestSolution->getFitness() << "\t\t" << improvement << "\t" << stdev << "\t" << ichange
////			<< "\t" << genChange << "\t" << bestChange->getFitness() << endl;
//	results += to_string(static_cast<long long>(gen+1)) + "," +
//				std::to_string(static_cast<long double>(bestSolutionOfPopulation->getFitness())) + "," +
//				std::to_string(static_cast<long double>(stdev)) + "," + to_string(static_cast<long long>(ichange)) + "," +
//				to_string(static_cast<long long>(genChange)) + "," +
//				std::to_string(static_cast<long long>(bestPerChange->getFitness())) + "," +
//				std::to_string(static_cast<long double>(muMax - muMin)) +"\n";
//
//	for (i = 0; i < pop.size(); i++){
//		delete pop[i];
//	}
//
//
//	ofstream myfile1;
//	myfile1.open(m_resultsPath);
//	myfile1 << results;
//	myfile1.close();
//
//	ofstream myfile2;
//	myfile2.open(m_modelPath);
//	myfile2 << trajectory;
//	myfile2.close();
//
//}
//
//void RKEDATrajectory::readModelPermutation(string file){
//	try{
//		ifstream indata;
//		char *line = new char[2048];
//		cout << file << endl;
//		indata.open(file.c_str(), ios::in);
//		int numOfPerms;
//		int i = 0, j=0;
//
//		// Read the content of the taillard file
//		while (!indata.eof()){
//			// Read the line of the file
//			indata.getline(line, 2048);
//			cout << line << endl;
//			if ((line != NULL) && (line[0] == '\0')){
//				break;
//			}else if (!Tools::strContains(line, ",")){
//				numOfPerms = atoi(line);
//
//				//BUILD JOB PROCESSING MATRIX
//				m_modePermutations = new int*[numOfPerms];
//				for (i = 0; i<numOfPerms; i++){
//					m_modePermutations[i] = new int[m_problemSize];
//				}
//				i = 0;
//			}else{
//				char *perm = strtok(line,",");
//				while (perm){
//					m_modePermutations[i][j] = atoi(perm);
//					perm = strtok(NULL,",");
//					j++;
//				}
//				i++;
//				j = 0;
//			}
//		}
//		indata.close();
//	}catch (exception& e){
//		cout << "Couldn't find file: " << file << endl;
//	}
//}
