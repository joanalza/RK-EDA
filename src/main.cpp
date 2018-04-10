///* 
// * File:   main.cpp
// * Author: mayowaayodele
// *
// * Created on 10 February 2017, 10:55
// */
//
//#include "stdafx.h"
#include <iostream>
#include <random>
#include <climits>
//
//#include "Add.h"
//#include "RK.h"

#include "Tools.h"
#include "RK.h"
#include "DPFSP.h"
using namespace std;
#include <stdio.h> 
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "EDAUtils.h"
#include "RKEDA.h"

int main(int argc, char* argv[]) {

	string fileDirectory, fileName, filePath, dynamicFolder, dynamicName, dynamicPath, resultsFolder, resultsName, resultsPath;
	int populationSize, FEs, truncSize, pSize, elitism;
	double minTemp, heating;
	unsigned long seed;

	int runs = 1;

	// If arguments are passed to the execution
	if (argc > 1) {
//		string input = "500 ./taillard_instances/ tai20_10_0.fsp ./dynamic/ dynProfile-noChange.txt -1 5 0 0.02 ./results/ 0.06 ";

		string temp = argv[1];
		stringstream(temp) >> populationSize;

		fileDirectory =  argv[2];
		fileName= argv[3];

		dynamicFolder =  argv[4] ;
		dynamicName = argv[5];

		temp = argv[6];
		stringstream(temp) >> FEs;

		temp = argv[7];
		stringstream(temp) >> truncSize;

		temp = argv[8];
		stringstream(temp) >> elitism;

		temp = argv[9];
		stringstream(temp) >> minTemp;

		resultsFolder = argv[10];

		temp = argv[11];
		stringstream(temp) >> heating;

		temp = argv[12];
		stringstream(temp) >> seed;

	} else {

//		string input = "500 ./taillard_instances/ tai50_5_0.fsp 220712150 50 0 0.05 ./results/ tai50_5_0P500T50V0.06e0run0.txt ";
		string input = "500 ./taillard_instances/ tai20_10_0.fsp ./dynamic/ dynProfile-noChange.txt -1 5 0 0.02 ./results/ 0.06 ";

		long found = 0;
		vector<int> pos;

		while (found < ULONG_MAX) {
			found = input.find(" ", found+1);
			pos.push_back(found);
		}

		string temp = input.substr(0, pos[0]);
		stringstream(temp) >> populationSize;

		temp = input.substr(pos[0] + 1, pos[1] - (pos[0] + 1));
		fileDirectory = temp;

		temp = input.substr(pos[1] + 1, pos[2] - (pos[1] + 1));
		fileName = temp;

		temp = input.substr(pos[2] + 1, pos[3] - (pos[2] + 1));
		dynamicFolder = temp;

		temp = input.substr(pos[3] + 1, pos[4] - (pos[3] + 1));
		dynamicName = temp;

		temp = input.substr(pos[4] + 1, pos[5] - (pos[4] + 1));
		stringstream(temp) >> FEs;

		temp = input.substr(pos[5] + 1, pos[6] - (pos[5] + 1));
		stringstream(temp) >> truncSize;

		temp = input.substr(pos[6] + 1, pos[7] - (pos[6] + 1));
		stringstream(temp) >> elitism;

		temp = input.substr(pos[7] + 1, pos[8] - (pos[7] + 1));
		istringstream(temp) >> minTemp;

		temp = input.substr(pos[8] + 1, pos[9] - (pos[8] + 1));
		resultsFolder = temp;

		temp = input.substr(pos[9] + 1, pos[10] - (pos[9] + 1));
		stringstream(temp) >> heating;

		pos.clear();
	}

	filePath = fileDirectory + fileName;
	dynamicPath = dynamicFolder + dynamicName;

	for (int i = 0; i < runs; i++){
		RKEDA* rkeda = new RKEDA(populationSize, filePath, dynamicPath, FEs, truncSize, elitism, resultsFolder, seed);
		rkeda->setResultsPath(fileName, dynamicName, minTemp, heating, i);
		rkeda->runAlgorithm(minTemp, heating);
	}

	return 0;
}
