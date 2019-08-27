/*
 * DPFSP.cpp
 *
 *  Created on: 10 Apr 2018
 *      Author: 1715818
 */

#define  _CRT_SECURE_NO_WARNINGS
#include "DPFSP.h"
#include "Tools.h"
#include "PFSP.h"
#include "DynPermP.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
using std::ifstream;
using std::ofstream;

DPFSP::DPFSP(){
}

DPFSP::~DPFSP(){
	for (int i = 0; i < m_machines; i++){
		delete[] m_processing_matrix[i];
	}
}

int DPFSP::ReadInstance(string filename){

	m_nextChangeIndex = 0;
	// Initialization of variables
	bool readMatrix = false;
	bool readDimension = false;
	char *line = new char[2048]; // variable for input value 
	string data = ""; 
	ifstream indata; 
	indata.open(filename.c_str(), ios::in); // +3

	// Read the content of the taillard file
	while (!indata.eof()){ 
		// Read the line of the file
		indata.getline(line, 2048); 
//		cout << line << endl;
		if (Tools::strContains(line, "number of jobs") == true && readMatrix == true){
			break;
		}else if (Tools::strContains(line, "number of jobs") == true){
			readDimension = true;
		}else if (readDimension == true){
			//char * pch;
			m_jobs = atoi(strtok(line, " "));
			//cout<<"JOB NUM: "<<JOB_NUM<<endl;
			m_machines = atoi(strtok(NULL, " "));
			//cout<<"MACHINE NUM: "<<MACHINE_NUM<<endl;
			readDimension = false;
		}else if (readMatrix){
			if (data == "")
				data = line; 
			else
				data = data + ' ' + line;
		}else if (Tools::strContains(line, "processing times :")){
			readMatrix = true;
		}
	}
	indata.close(); 

	//SAVE PROBLEM SIZE
	m_problemSize = m_jobs;

	//BUILD JOB PROCESSING MATRIX
	m_processing_matrix = new int*[m_machines]; 
	for (int i = 0; i<m_machines; i++){ 
		m_processing_matrix[i] = new int[m_jobs];
	}

	//BUILD IDENTITY PERMUTATION
	m_identityPerm = new int[m_jobs]; 
	for (int i = 0; i<m_jobs; i++){
		m_identityPerm[i] = i;
	}

	//FILL JOB PROCESSING MATRIX
	istringstream iss(data);
	int i = 0;
	int j = 0;
	do{
		string sub; 
		iss >> sub; // +3
		if (sub != ""){
			//save distance in distances matrix. Save negative distance in order to minimize m_fitness instead of
			//maximize.
			m_processing_matrix[i][j] = atoi(sub.c_str());
			if (j == m_jobs - 1){
				i++;
				j = 0;
			}else{
				j++;
			}
		}else{
			break;
		}
	} while (iss);

	delete[] line;
	return(m_jobs);
} 

void DPFSP::ReadDynamic(string dynamic){
	// Read the content of the dynamic file
	try {
		ifstream indata;
		char *line = new char[2048]; // variable for input value
		char *perm;
		int *aux_identityPermutations; 
		//		cout << m_dynamicProfilePath << endl;
		indata.open(dynamic.c_str(), ios::in);

		int i = 0, j = 0;

		while (!indata.eof()) {
			// Read the line of the file
			indata.getline(line, 2048);
			//				cout << line << endl;
			if ((line != NULL) && (line[0] == '\0')) {
				break;
			}
			else if (!Tools::strContains(line, ";")) {
				m_numChanges = atoi(line); 
				m_normalisedChangeGenerations = new double[m_numChanges];
				i = 0;
			}
			else {
				//char *data = ;
				aux_identityPermutations = new int[m_problemSize]; 
				m_normalisedChangeGenerations[i] = atof(strtok(line, ";"));
				//data = ;
				perm = strtok(strtok(NULL, ";"), ",");
				while (perm) {
					aux_identityPermutations[j] = atoi(perm);
					//					Tools::printPermutation(identityPermutations, m_problemSize);
					perm = strtok(NULL, ",");
					j++;
				}
				m_identityPermutations.push_back(aux_identityPermutations); //+1
				//				Tools::printPermutation(identityPermutations, m_problemSize);
				i++;
				j = 0;
			}
		}
		delete[] perm;
		delete[] line;
		indata.close();
	}
	catch (exception& e) {
		cout << "Couldn't find file: " << dynamic << endl;
	}

}

double DPFSP::EvaluateFSPMakespan(int *genes){
	//    EVALUATIONS++;
	double fitness = 0;
	double* timeTable = new double[m_machines];

	for (int j = 0; j<m_machines; j++){
		timeTable[j] = 0;
	}
	for (int z = 0; z<m_jobs; z++){
		int job = genes[z];
		//cout<<"Job "<<job<<endl;
		for (int machine = 0; machine<m_machines; machine++){
			double processingTime = m_processing_matrix[machine][job];
			if (machine == 0){
				timeTable[machine] += processingTime;
			}else{
				if (timeTable[machine - 1]<timeTable[machine]){
					timeTable[machine] = timeTable[machine] + processingTime;
				}else{
					timeTable[machine] = timeTable[machine - 1] + processingTime;
				}
			}
		}
	}

	fitness = timeTable[m_machines - 1];
	delete[] timeTable;// ###ORC
	return fitness;
}

double DPFSP::Evaluate(int *genes){
//	Tools::PrintArray(genes, m_jobs, "Perm1: ");
	m_evaluations++;
	int *timeTable = new int[m_machines];
	int i, j, z, job, first_gene;
	int machine;
	int prev_machine = 0;
	double fitness;
	// int[] m_aux= new int[m_jobs];

	for (i = 0; i < m_machines; i++)
		timeTable[i] = 0;
	// int first_gene=genes[0];
	first_gene = genes[m_identityPerm[0]];

	timeTable[0] = m_processing_matrix[0][first_gene];
	for (j = 1; j < m_machines; j++) {
		timeTable[j] = timeTable[j - 1] + m_processing_matrix[j][first_gene];
	}

	fitness = timeTable[m_machines - 1];
	for (z = 1; z < m_jobs; z++) {
//		job = genes[z];
		job = genes[m_identityPerm[z]];

		// machine 0 is always incremental, so:
		timeTable[0] += m_processing_matrix[0][job];
		prev_machine = timeTable[0];
		for (machine = 1; machine < m_machines; machine++) {
			timeTable[machine] = max(prev_machine, timeTable[machine]) + m_processing_matrix[machine][job];
			prev_machine = timeTable[machine];
		}

		fitness += timeTable[m_machines - 1];
	}

	delete[] timeTable;// ###ORC
	// return -m_fitness;
	return fitness;
}

int DPFSP::getNumberofEvaluation(){
	return m_evaluations;
}

double DPFSP::getChangeStep(int changePeriod){
//	Tools::printarray(m_idenityChangesPercentage, m_changes);
	return m_normalisedChangeGenerations[changePeriod - 1];
}

int DPFSP::GetProblemSize(){
	return(m_jobs);
}

int DPFSP::getNumOfChanges(){
	return(m_numChanges);
}

string DPFSP::getDistance(string dynamic){
	size_t extension = dynamic.find_last_of(".");
	size_t distIndex = dynamic.find_last_of("-");
	string rawname = dynamic.substr(distIndex + 1, extension - distIndex - 1);
	return rawname.substr(0, rawname.length());
}

string DPFSP::getDistanceType(string dynamic){
	string metric;
	string distance = getDistance(dynamic);
	return distance.substr(0, 1);
}

string DPFSP::getDistanceMagnitude(string dynamic){
	string distance = getDistance(dynamic);
	return distance.substr(1, distance.length());
}

//void DPFSP::setIdentityPermutationChanges(){
//	try{
//
//		ifstream indata;
//		char *perm;
//		char *line = new char[2048]; // variable for input value
//		int *aux_identityPermutations;
////		cout << m_dynamicProfilePath << endl;
//		indata.open(m_dynamicProfilePath.c_str(), ios::in);
//
//		int i = 0, j=0;
//
//		while (!indata.eof()){
//			// Read the line of the file
//			indata.getline(line, 2048);
////				cout << line << endl;
//			if ((line != NULL) && (line[0] == '\0')){
//				break;
//			}else if (!Tools::strContains(line, ";")){
//				m_numChanges = atoi(line);
//				m_normalisedChangeGenerations = new double[m_numChanges];
//				i = 0;
//			}else{
//				//char *data = ;
//				aux_identityPermutations = new int[m_problemSize];
//				m_normalisedChangeGenerations[i] = atof(strtok(line, ";"));
//				//data = ;
//				perm = strtok(strtok(NULL, ";"),",");
//				while (perm){
//					aux_identityPermutations[j] = atoi(perm);
////					Tools::printPermutation(identityPermutations, m_problemSize);
//					perm = strtok(NULL,",");
//					j++;
//				}
//				m_identityPermutations.push_back(aux_identityPermutations);
////				Tools::printPermutation(identityPermutations, m_problemSize);
//				i++;
//				j = 0;
//			}
//		}
//		for (int k = 0; k<m_identityPermutations.size(); k++)
//			Tools::printPermutation(m_identityPermutations.at(k), m_problemSize);
//		cout << "PRINT" << endl;
//		delete[] perm;
//		delete[] line;
//		indata.close();
//
//
//	  }catch (exception& e){
//	    cout << "Couldn't find file: " << m_dynamicProfilePath << endl;
//	  }
//}

/*bool DPFSP::changeIdentityPermutation(int fes, int maxfes){
	if (m_nextChangeIndex < m_numChanges){
		int nextChangeFes = (int)rint(m_normalisedChangeGenerations[m_nextChangeIndex]*(double)maxfes);
		if(fes>=nextChangeFes){
			m_identityPerm = m_identityPermutations[m_nextChangeIndex];
			Tools::printPermutation(m_identityPerm, m_problemSize);
			m_nextChangeIndex++;
			return true;
		}
	}
	return false;
}*/

bool DPFSP::detectChange(RK *sol1,RK *sol2){
//	cout << Tools::perm2str(sol1->getPermutation(), sol1->getProblemSize()) << "--" << sol1->getFitness() << "--" << sol1->getProblemSize() <<endl;
//	cout << Tools::perm2str(sol2->getPermutation(), sol2->getProblemSize()) << "--" << sol2->getFitness() << "--" << sol2->getProblemSize() <<endl;
//	cout << Tools::areEqual(sol1->getPermutation(), sol2->getPermutation(),sol1->getProblemSize(),sol2->getProblemSize()) << endl;
	bool changed = Tools::areEqual(sol1->getPermutation(), sol2->getPermutation(),sol1->getProblemSize(),sol2->getProblemSize());
	if (changed && (sol1->getFitness() != sol2->getFitness())){
		m_identityPerm = m_identityPermutations[m_nextChangeIndex];
		m_nextChangeIndex++;
		return true;
	}
	return false;
}

int* DPFSP::getIdentityPermutation(int period){
	return m_identityPermutations[period];
}
