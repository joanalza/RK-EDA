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
//#include "random.h"

#include <ctime>
#include <string>
#include <iostream>


 /*
 RKEDA::RKEDA (int probSize, int popSize, int trunSize, double stdev, int FEs, std::string filename){
	  problemSize =probSize ;
	 populationSize = popSize;
	 truncationSize = trunSize;
	variance = stdev;
	fitnessEvaluations = FEs;
	std::string fileName = filename;
}*/

RKEDA::RKEDA(int probSize, int popSize, int trunSize, double stdev, int FEs, std::string filename, std::string results, unsigned long theseed) {
	problemSize = probSize;
	populationSize = popSize;
	truncationSize = trunSize;
	variance = stdev;
	fitnessEvaluations = FEs;
	fileName = filename;
	fileNameResults = results;
	seed = theseed;
}

//void RKEDA::runAlgorithm(string fileName, string fileNameResults) {
//
//    ofstream myfile;
//    //  myfile.open ("./results/fsp" + std::to_string(truncationSize) +".txt");
//
//    myfile.open(fileNameResults);
//    string results = "FileName \t Solution \tFitness \t err \t FEs \n";
//    string results1 = "FileName \t Solution \tFitness \t err \t FEs \n";
//
//
//    int numberofGenerations = (fitnessEvaluations / populationSize);
//    cout << "number of gens: " << numberofGenerations << endl;
//    vector<RK*> pop = e.initialPopulationRK(problemSize, populationSize, fileName);
//    RK* bestSolution = e.getBestSolution();
//    cout << "end initialisation" << endl;
//    for (int i = 0; i <= numberofGenerations; i++) {
//        if (e.getNumberofEvaluations() >= fitnessEvaluations) {
//            break;
//        }
//        double coolingParameter = 1 - ((double) i / numberofGenerations);
//        variance = variance * coolingParameter;
//        // cout<<"stdev: "<<variance<<endl;
//        double* matrix = e.getPM(pop, truncationSize, problemSize);
//
//        vector<RK*> population = e.getPopulationRK(matrix, variance, problemSize, populationSize);
//        pop.clear();
//        pop = population;
//        if (bestSolution->getFitness() > e.getBestSolution()->getFitness()) {
//            bestSolution = e.getBestSolution();
//            cout << "gen: " + to_string(i) << endl;
//            cout << bestSolution->getFitness() << endl;
//            results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(e.getNumberofEvaluations()) + "\n";
//            //		   results += fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(e.getNumberofEvaluations()) + "\n";
//            results += results1;
//        }
//
//        //	 cout <<results<<endl;
//        myfile << results;
//        myfile.close();
//        delete[] matrix;
//        population.clear();
//        delete bestSolution;
//    }
//
//    ofstream myfile1;
//    myfile1.open(fileNameResults.replace(fileNameResults.end() - 4, fileNameResults.end(), "all.txt"));
//    myfile1 << results1;
//    myfile1.close();
//    cout << "ended" << endl;
//
//}

void RKEDA::runAlgorithm1(string fileName, string fileNameResults) {

	//    ofstream myfile;
	//  myfile.open ("./results/fsp" + std::to_string(truncationSize) +".txt");

	//    myfile.open(fileNameResults);
	string results = "FileName \t Solution \tFitness \t err \t FEs \n";
	string results1 = "FileName \t Solution \tFitness \t err \t FEs \n";

	int noOfEvals = 0;
	int numberofGenerations = (fitnessEvaluations / populationSize);
	//    vector<RK*> pop = e.initialPopulationRK(problemSize, populationSize, fileName);
	RK* bestSolution;
	fsp.ReadInstance(fileName);
	vector<RK*> pop(populationSize);
	vector<RK*> population(populationSize);

	double r, r2, mean;
	//cout << "Main Process with pid : " << getpid();

	Random *random;



	if (seed == -1) {
		random = new Random(new SRandom(time(NULL)));
	}
	else {
		random = new Random(new SRandom(seed));
	}

	
	//Random *random = new Random(new SRandom(time(NULL)));
	//Random *random = new Random(new SRandom(rand()));
	//std::this_thread::get_id();
	time_t  starttime = time(0);
	//cout << "starttime:" << starttime << endl;

	srand(seed);

	cout << "fes; gen; stdev; best" << endl;

	// Initialize the first population
	for (int i = 0; i < populationSize; i++) {
		//cout << "i: " << i << endl;

		// Create the random keys of the individuals
		double* childAct = new double[problemSize];
		for (int x = 0; x < problemSize; x++) {
			r = ((double)rand() / (RAND_MAX));
			childAct[x] = r;
		}
		RK* child = new RK(childAct, problemSize);

		child->setPermutation(e.randomKeyToAL(childAct, problemSize));
		child->normalise();
		child->setFitness(fsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
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
		//delete[] childAct;
		//        delete child;
	}



	//cout << "end initialisation" << endl;
	for (int i = 0; i < numberofGenerations; i++) {
		if (noOfEvals >= fitnessEvaluations) {
			break;
		}
		double coolingParameter = 1 - ((double)i / numberofGenerations);
		double temp_variance = variance * coolingParameter;
		//cout << noOfEvals << ", " << i << ", " << temp_variance << ", " << bestSolution->getFitness() <<endl;
		double* matrix = new double[problemSize];
		matrix = e.getPM(pop, truncationSize, problemSize);

		//        pop.clear();
		//std::random_device generator;
		//std::mt19937 gen(generator());

		for (int j = 0; j < populationSize; j++) {

			double* childAct = new double[problemSize];
			for (int x = 0; x < problemSize; x++) {
				//             double r2 = r.nextGaussian() * currentModel[x][1] + currentModel[x][0];
				mean = matrix[x];
				//std::normal_distribution<double> distribution(mean, variance);
				//std::uniform_real_distribution<double> distribution(mean, variance);
				r2 = random->normal(temp_variance) + mean;

				//r2 = distribution(generator);
				childAct[x] = r2;

			}
			RK* child = new RK(childAct, problemSize);
			//            RK child(childAct, problemSize);
			child->setPermutation(e.randomKeyToAL(childAct, problemSize));
			child->normalise();
			child->setFitness(fsp.EvaluateFSPTotalFlowtime(child->getPermutation()));
			noOfEvals++;
			
			if (noOfEvals >= fitnessEvaluations) {
				break;
			}
			population[j] = child;
			if (bestSolution->getFitness() > child->getFitness()) {
				//bestSolution = child;
				//delete[] bestSolution;
				//delete[] bestSolution->permutation;
				//delete[] bestSolution->randomkeys;
				bestSolution = child->Clone2();

				cout << noOfEvals << "; " << (i+1) << "; " << temp_variance << "; " << bestSolution->getFitness() << "; [" << bestSolution->getPermutationAsString() << "]" << endl;

				//cout << "gen: " + to_string(i) << endl;
				//cout << bestSolution->getFitness() << endl;
				//results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(noOfEvals) + "\n"; // ##C++11
				results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(static_cast<long double>(bestSolution->getFitness())) + "\t" + "0" + "\t" + to_string(static_cast<long long>(noOfEvals)) + "\n"; // ##C++0x
	//                results += results1;
			}

			 //delete[] childAct;
			 //child ->~RK();
			 //delete child;
		}

		for (int i = 0; i < pop.size(); i++)
		{
			//pop[i]->~RK();
			delete pop[i];
		}
		pop = population;
		//        population.clear();
		//        cout << "gen: " + to_string(i) << endl;
		//        cout << bestSolution->getFitness() << endl;
		//        results1 = fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(noOfEvals) + "\n";
		//		   results += fileName + "\t" + e.perm2str(bestSolution->getPermutation(), problemSize) + "\t" + std::to_string(bestSolution->getFitness()) + "\t" + "0" + "\t" + to_string(e.getNumberofEvaluations()) + "\n";
		//        results += results1;

		//	 cout <<results<<endl;
		//        myfile << results;
		//        myfile.close();
		delete[] matrix;
		//        population.clear();
	}

	time_t  endtime = time(0);
	//cout << "totaltime: " << (endtime - starttime) << " -- timePerEval: " << ((endtime - starttime) / noOfEvals) << endl;
	//cout << noOfEvals << ", " << numberofGenerations << ", " << variance << ", " << bestSolution->getFitness() << endl;


	//for (int i = 0; i < population.size(); i++)
	//{
	//	delete population[i];
	//	//population[i]->~RK();
	//}

	//delete bestSolution;

	//fsp.~PFSP();
	ofstream myfile1;
	myfile1.open(fileNameResults);

	//myfile1.open(fileNameResults.replace(fileNameResults.end() - 4, fileNameResults.end(), "all.txt"));
	myfile1 << results1;
	myfile1.close();
	//cout << "ended" << endl;

}

