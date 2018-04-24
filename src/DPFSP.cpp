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
	delete[] m_idenityChangesPercentage;
	for (int i = 0; i < m_machines; i++){
		delete[] m_processing_matrix[i];
	}
	for (int j = 0; j < m_changes; j++){
		delete[] m_identityPermutations[j];
	}
}

int DPFSP::ReadTaillardInstance(string filename, string dynamic){

	m_nextChangeIndex = 0;
	// Initialization of variables
	bool readMatrix = false;
	bool readDimension = false;
	char *line = new char[2048]; // variable for input value
	string data = "";
	ifstream indata;
	indata.open(filename.c_str(), ios::in);

	// Read the content of the dynamic file
	m_dynamicProfilePath = dynamic;

	// Read the content of the taillard file
	while (!indata.eof()){
		// Read the line of the file
		indata.getline(line, 2048);
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

	setIdentityPermutationChanges();

	return(m_jobs);
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

int DPFSP::EvaluateFSPTotalFlowtime(int *genes){
	m_evaluations++;
	int *timeTable = new int[m_machines];
	// int[] m_aux= new int[m_jobs];

	for (int i = 0; i < m_machines; i++)
		timeTable[i] = 0;

	int j, z, job;
	int machine;
	int prev_machine = 0;

	// int first_gene=genes[0];
	int first_gene = m_identityPermutation[genes[0]];

	timeTable[0] = m_processing_matrix[0][first_gene];
	for (j = 1; j < m_machines; j++) {
		timeTable[j] = timeTable[j - 1] + m_processing_matrix[j][first_gene];
	}

	int fitness = timeTable[m_machines - 1];
	for (z = 1; z < m_jobs; z++) {
		 job=genes[z];
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

int DPFSP::getProblemSize(){
	return(m_jobs);
}

void DPFSP::setIdentityPermutationChanges(){
	try{

		ifstream indata;
		char *line = new char[2048]; // variable for input value
		indata.open(m_dynamicProfilePath.c_str(), ios::in);

		int i = 0, j=0;

		while (!indata.eof()){
				// Read the line of the file
				indata.getline(line, 2048);
				if ((line != NULL) && (line[0] == '\0')){
					break;
				}else if (!m_t.strContains(line, ";")){
					m_changes = atoi(line);

					//BUILD JOB PROCESSING MATRIX
					m_idenityChangesPercentage = new double[m_changes];
					m_identityPermutations = new int*[m_changes];
					for (int i = 0; i<m_changes; i++){
						m_identityPermutations[i] = new int[m_jobs];
					}
				}else{
					//char *data = ;
					m_idenityChangesPercentage[i] = atof(strtok(line, ";"));
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
	bool hasChangedOccured = false;
	if (m_nextChangeIndex < m_changes){
		int nextChangeFes = (int)rint(m_idenityChangesPercentage[m_nextChangeIndex]*(double)maxfes);
		if(fes>=nextChangeFes){
			m_identityPermutation = m_identityPermutations[m_nextChangeIndex];
			m_nextChangeIndex++;
			cout << "##################################################" << endl;
			cout << "[" << m_t.perm2str(m_identityPermutation, m_jobs) << "]" << endl;
			cout << "##################################################" << endl;
			hasChangedOccured = true;
		}
	}
	return hasChangedOccured;
}
