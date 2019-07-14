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
	delete[] m_identityPermutation;
	delete[] m_identityChangesPercentage;
	for (int i = 0; i < m_machines; i++){
		delete[] m_processing_matrix[i];
	}
	for (int j = 0; j < m_changes; j++){
		delete[] m_identityPermutations[j];
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
	cout << filename << endl;
	indata.open(filename.c_str(), ios::in);

	// Read the content of the taillard file
	while (!indata.eof()){
		// Read the line of the file
		indata.getline(line, 2048);
		cout << line << endl;
		if (m_t.strContains(line, "number of jobs") == true && readMatrix == true){
			break;
		}else if (m_t.strContains(line, "number of jobs") == true){
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
		}else if (m_t.strContains(line, "processing times :")){
			readMatrix = true;
		}
	}
	indata.close();

	//BUILD JOB PROCESSING MATRIX
	m_processing_matrix = new int*[m_machines];
	for (int i = 0; i<m_machines; i++){
		m_processing_matrix[i] = new int[m_jobs];
	}

	//BUILD IDENTITY PERMUTATION
	m_identityPermutation = new int[m_jobs];
	for (int i = 0; i<m_jobs; i++){
		m_identityPermutation[i] = i;
	}

	//FILL JOB PROCESSING MATRIX
	istringstream iss(data);
	int i = 0;
	int j = 0;
	do{
		string sub;
		iss >> sub;
		if (sub != ""){
			//save distance in distances matrix. Save negative distance in order to minimize fitness instead of
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

	return(m_jobs);
}

void DPFSP::ReadDynamic(string dynamic){

	// Read the content of the dynamic file
	m_dynamicProfilePath = dynamic;

	setIdentityPermutationChanges();

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
	first_gene = m_identityPermutation[genes[0]];

	timeTable[0] = m_processing_matrix[0][first_gene];
	for (j = 1; j < m_machines; j++) {
		timeTable[j] = timeTable[j - 1] + m_processing_matrix[j][first_gene];
	}

	fitness = timeTable[m_machines - 1];
	for (z = 1; z < m_jobs; z++) {
//		job = genes[z];
		job = m_identityPermutation[genes[z]];

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
	// return -fitness;
	return fitness;
}

int DPFSP::getNumberofEvaluation(){
	return m_evaluations;
}

double DPFSP::getChangeStep(int changePeriod){
//	Tools::printarray(m_idenityChangesPercentage, m_changes);
	return m_identityChangesPercentage[changePeriod - 1];
}

int DPFSP::GetProblemSize(){
	return(m_jobs);
}

int DPFSP::getNumOfChanges(){
	return(m_changes);
}

string DPFSP::getDistance(string dynamic){
	size_t extension = dynamic.find_last_of(".");
	size_t distIndex = dynamic.find_last_of("-");
	string rawname = dynamic.substr(distIndex + 1, extension - distIndex - 1);
	return rawname.substr(0, rawname.length());
}

string DPFSP::getDistanceType(string dynamic){
	string metric;
	int value;
	string distance = getDistance(dynamic);
	return distance.substr(0, 1);
}

string DPFSP::getDistanceMagnitude(string dynamic){
	string distance = getDistance(dynamic);
	return distance.substr(1, distance.length());
}

void DPFSP::setIdentityPermutationChanges(){
	try{

		ifstream indata;
		char *line = new char[2048]; // variable for input value
		cout << m_dynamicProfilePath << endl;
		indata.open(m_dynamicProfilePath.c_str(), ios::in);

		int i = 0, j=0;

		while (!indata.eof()){
				// Read the line of the file
				indata.getline(line, 2048);
				cout << line << endl;
				if ((line != NULL) && (line[0] == '\0')){
					break;
				}else if (!m_t.strContains(line, ";")){
					m_changes = atoi(line);

					//BUILD JOB PROCESSING MATRIX
					m_identityChangesPercentage = new double[m_changes];
					m_identityPermutations = new int*[m_changes];
					for (i = 0; i<m_changes; i++){
						m_identityPermutations[i] = new int[m_jobs];
					}
					i = 0;
				}else{
					//char *data = ;
					m_identityChangesPercentage[i] = atof(strtok(line, ";"));
					//data = ;
					char *perm = strtok(strtok(NULL, ";"),",");
					while (perm){
						m_identityPermutations[i][j] = atoi(perm);
						perm = strtok(NULL,",");
						j++;
					}
					i++;
					j = 0;
				}
		}
		indata.close();


	  }catch (exception& e){
	    cout << "Couldn't find file: " << m_dynamicProfilePath << endl;
	  }
}

bool DPFSP::changeIdentityPermutation(int fes, int maxfes){
	if (m_nextChangeIndex < m_changes){
		int nextChangeFes = (int)rint(m_identityChangesPercentage[m_nextChangeIndex]*(double)maxfes);
		if(fes>=nextChangeFes){
			m_identityPermutation = m_identityPermutations[m_nextChangeIndex];
			m_nextChangeIndex++;
			return true;
		}
	}
	return false;
}

bool DPFSP::detectChange(RK *sol1,RK *sol2){
//	cout << Tools::perm2str(sol1->getPermutation(), sol1->getProblemSize()) << "--" << sol1->getFitness() << "--" << sol1->getProblemSize() <<endl;
//	cout << Tools::perm2str(sol2->getPermutation(), sol2->getProblemSize()) << "--" << sol2->getFitness() << "--" << sol2->getProblemSize() <<endl;
//	cout << Tools::areEqual(sol1->getPermutation(), sol2->getPermutation(),sol1->getProblemSize(),sol2->getProblemSize()) << endl;
	bool changed = Tools::areEqual(sol1->getPermutation(), sol2->getPermutation(),sol1->getProblemSize(),sol2->getProblemSize());
	if (changed && (sol1->getFitness() != sol2->getFitness())){
		m_identityPermutation = m_identityPermutations[m_nextChangeIndex];
		m_nextChangeIndex++;
		return true;
	}
	return false;
}

int* DPFSP::getIdentityPermutation(int period){
	return m_identityPermutations[period];
}
