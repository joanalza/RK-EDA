/*
 * Population.h
 *
 *  Created on: 27 May 2019
 *      Author: ja9508
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include <vector>       // std::vector
#include <random>       // std::vector
using namespace std;
#include "DynPermP.h"
#include "RK.h"

class Population {
	public:
		Population();
		virtual ~Population();



	private:
		int popSize;
		vector<RK*> population;
		RK *bestSolution;
		bool minimisationProblem;
		double average;

};

#endif /* POPULATION_H_ */
