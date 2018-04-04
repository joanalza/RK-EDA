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
using namespace std;
#include <stdio.h> 
//#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "EDAUtils.h"
#include "RKEDA.h"

int main (int argc, char* argv[]) {
	
	string folderName, fileName, resultsFolder, resultsName;
	int populationSize, FEs, truncSize, pSize;
	double stdev;
	unsigned long seed;

	// If arguments are passed to the execution
	if (argc > 1) {
		
		string temp = argv[1];
		stringstream(temp) >> populationSize;

		folderName = argv[2];
		fileName = folderName + argv[3];

		temp = argv[4];
		stringstream(temp) >> FEs;

		temp = argv[5];
		stringstream(temp) >> truncSize;

		temp = argv[6];
		stringstream(temp) >> pSize;

		temp = argv[7];
		stringstream(temp) >> stdev;

		resultsFolder = argv[8];
		resultsName = resultsFolder + argv[9];
		
		temp = argv[10];
		stringstream(temp) >> seed;


		/*cout << "argc: " << argc << endl;
		cout << "argv0: " << argv[0] << endl;
		cout << "argv0: " << argv[1] << endl;
		cout << "argv0: " << argv[2] << endl;
		cout << "argv0: " << argv[3] << endl;*/

	}
	else {
		//int FEs = 0, pSize = 0, populationSize = 0, truncSize = 0;
		//double stdev = 0;
		//string fileName = "", resultsName = "";
		
		//char input[400] = "500 ../src/taillard_instances/ tai50_5_0.fsp 5000 50 0 0.05 ../results/ tai50_5_0P500T50V0_06e0run0.txt";
		//string input = "500 ./taillard_instances/ tai50_5_0.fsp 5000 50 0 0.05 ./results/ tai50_5_0P500T50V0_06e0run0.txt";

		string input = "500 ./taillard_instances/ tai50_5_0.fsp 220712150 50 0 0.05 ./results/ tai50_5_0P500T50V0.06e0run0.txt ";
		// char input[400] = "1000 ../src/taillard_instances/ tai100_5_0.fsp 47175960 100 0 0.05 ../results/ tai100_5_0P1000T100V0.05e0run0.txt";
		//char input[400] = "5000 ../src/taillard_instances/ tai500_20_9.fsp 260316750 500 0 0.05 ../results/ tai500_20_9P5000T500V0.05e0run19.txt";

		long found = 0;
		vector<int> pos;

		while (found < ULONG_MAX) {
			found = input.find(" ", found + 1);
			cout << "found: " << found << endl;
			pos.push_back(found);
		}

		string temp = input.substr(0, pos[0]);
		//cout << "pop: " << temp << endl;
		stringstream(temp) >> populationSize;
		//cout << "pop: " << populationSize << endl;

		temp = input.substr(pos[0] + 1, pos[1] - (pos[0] + 1));
		string folderName = temp;
		//cout << "pbfolder: " << temp << endl;

		temp = input.substr(pos[1] + 1, pos[2] - (pos[1] + 1));
		fileName = folderName + temp;
		//cout << "pbfile: " << temp << endl;

		temp = input.substr(pos[2] + 1, pos[3] - (pos[2] + 1));
		//cout << "fes: " << temp << endl;
		stringstream(temp) >> FEs;


		temp = input.substr(pos[3] + 1, pos[4] - (pos[3] + 1));
		//cout << "trunc: " << temp << endl;
		stringstream(temp) >> truncSize;

		temp = input.substr(pos[4] + 1, pos[5] - (pos[4] + 1));
		//cout << "IGNORE: " << temp << endl;

		temp = input.substr(pos[5] + 1, pos[6] - (pos[5] + 1));
		//cout << "var: " << temp << endl;
		istringstream(temp) >> stdev;
		//cout << "var2: " << temp << endl;

		temp = input.substr(pos[6] + 1, pos[7] - (pos[6] + 1));
		//cout << "resultFolder: " << temp << endl;
		string resultsFolder = temp;

		temp = input.substr(pos[7] + 1, pos[8] - (pos[7] + 1));
		//cout << "resultFile: " << temp << endl;
		resultsName = resultsFolder + temp;

		pos.clear();
		pSize = truncSize;
	}






   RKEDA rkeda (pSize,populationSize, truncSize,stdev, FEs, fileName, resultsName, seed);
   //cout<<"start"<<endl;
   rkeda.runAlgorithm1(fileName, resultsName);

  return 0;
}
