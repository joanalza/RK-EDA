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
#include "DPFSP.h"
#include "RK.h"
using namespace std;
#include <stdio.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "EDAUtils.h"
#include "RKEDA.h"
#include "Tools.h"

int main(int argc, char* argv[]) {

	string fileDirectory, fileName, filePath, dynamicFolder, dynamicName, dynamicPath, resultsFolder, resultsPath, saveAs, modelPath; //+10
	int populationSize, generations, truncSize, elitism, restart;
	double minTemp, heating;
	unsigned long seed;

	// If arguments are passed to the execution
	if (argc > 1) {

//		cout << "Arguments" << endl;
		string temp = argv[1]; //+1
		stringstream(temp) >> populationSize; //+7

		fileDirectory =  argv[2]; //+1
		fileName= argv[3];
		dynamicFolder =  argv[4] ;
		dynamicName = argv[5]; //+1

		temp = argv[6];
		stringstream(temp) >> generations;

		temp = argv[7];
		stringstream(temp) >> truncSize;

		temp = argv[8];
		stringstream(temp) >> elitism;

		temp = argv[9];
		stringstream(temp) >> minTemp;

		temp = argv[10];
		stringstream(temp) >> heating;

		resultsFolder = argv[11];

		saveAs= argv[12];
		saveAs.insert(0,"progress-");
		saveAs.append(".csv");

		temp = argv[13];
		stringstream(temp) >> seed;

		temp = argv[14];
		stringstream(temp) >> restart;

	} else {

//		cout << "Without arguments" << endl;
//		string input = "500 ./taillard_instances/ tai50_5_0.fsp 220712150 50 0 0.05 ./results/ tai50_5_0P500T50V0.06e0run0.txt ";

		string input = "200 taillard_instances/ tai20_20_0.fps dynamic/cayley/20/ dynProfile_n20_c10_C1__1.txt 20000 20 1 0.15 0 results/tai20_0/cayley/ tai20_20_0-dynProfile_n20_c10_C1__1-LC0.15_0-r0-elt1--1 812297940 0";
//		string input = "200 taillard_instances/ tai20_5_9.fsp dynamic/ dynProfile-n20-c10-Cayley10.txt 4000000 20 0 0.15 results/ tai20_5_9-dynProfile-n20-c10-Cayley10-currentbest_0.15-elt0--9 9 0";
//		string input = "500 taillard_instances/ tai50_10_0.fsp dynamic/ dynProfile-n50-c10-Cayley45.txt -1 5 0 0.0 results/ tai50_10_0-dynProfile-noChange-currentbest_0.0_0.2-elt0--9 0.2 1 ";


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
		stringstream(temp) >> generations;

		temp = input.substr(pos[5] + 1, pos[6] - (pos[5] + 1));
		stringstream(temp) >> truncSize;

		temp = input.substr(pos[6] + 1, pos[7] - (pos[6] + 1));
		stringstream(temp) >> elitism;

		temp = input.substr(pos[7] + 1, pos[8] - (pos[7] + 1));
		istringstream(temp) >> minTemp;

		temp = input.substr(pos[8] + 1, pos[9] - (pos[8] + 1));
		stringstream(temp) >> heating;

		temp = input.substr(pos[9] + 1, pos[10] - (pos[9] + 1));
		resultsFolder = temp;

		temp = input.substr(pos[10] + 1, pos[11] - (pos[10] + 1));
		saveAs= temp;

		temp = input.substr(pos[11] + 1, pos[12] - (pos[11] + 1));
		stringstream(temp) >> seed;

		temp = input.substr(pos[12] + 1, pos[13] - (pos[12] + 1));
		stringstream(temp) >> restart;

		pos.clear();
	}

	filePath = fileDirectory + fileName; // +1
	dynamicPath = dynamicFolder + dynamicName; // +1
	resultsPath = resultsFolder + saveAs; // +1

	RKEDA* rkeda = new RKEDA(populationSize, filePath, dynamicPath, generations, truncSize, elitism, resultsPath, seed, restart); //+8
	rkeda->runAlgorithm(minTemp, heating); //+21
	delete rkeda; //-8
	
	return 0; // -16
}
