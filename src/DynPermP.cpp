/*
 *  PBP.cpp
 *  RankingEDAsCEC
 *
 *  Created by Josu Ceberio Uribe on 7/11/13.
 *  Copyright 2013 University of the Basque Country. All rights reserved.
 *
 */

#include "DynPermP.h"
#include "Tools.h"

/*
 * The constructor.
 */
DynPermP::DynPermP()
{
	m_nextChangeIndex = 0;
}

/*
 * The destructor. It frees the memory allocated.
 */
DynPermP::~DynPermP()
{
	delete[] m_identityPerm;
	delete[] m_normalisedChangeGenerations ;
	for (int i = 0; i < m_identityPermutations.size() - 1; i++) {
		delete[] m_identityPermutations[i];
	}
	m_identityPermutations.clear();
}


void DynPermP::setIdentityPermutationChanges(string dynamicfilename) {
	
	//cout << "dynProfilePath" << dynamicfilename << endl;
	//cout << "dynProfilePath2" << dynamicfilename.c_str() << endl;

	std::ifstream infile(dynamicfilename.c_str());
	std::string line;
	int countlines = 0;
	
	while (std::getline(infile, line))
	{
		
		std::istringstream iss(line);
		if (countlines == 0) { 
			//sscanf(line.c_str(), "%d", &nChanges);
			stringstream(line) >> m_numChanges;
			m_normalisedChangeGenerations = new double[m_numChanges];
			//identityPermutations = new vector<int*>();
		/*}else if (countlines == 1) {
				//sscanf(line.c_str(), "%d", &pbsize);
				stringstream(line) >> pbsize;
				int* temp_array = new int[pbsize];
				for (int i = 0; i < pbsize; i++) {
					temp_array[i] = i;
				}
				identityPerm = temp_array;
				delete[] temp_array;*/
		}else{
			std::string delimiter = ";";
			int point1 = line.find(delimiter);
			std::string part0 = line.substr(0, point1);
			std::string part1 = line.substr(point1+1,line.length());
			//sscanf(part0.c_str(), "%d", &identityPermutationChanges[countlines - 2]);
			stringstream(part0) >> m_normalisedChangeGenerations[countlines - 1];
			size_t pos = 0;
			std::string delimiter2 = ",";
			std::string token;
			int* temparray = new int[m_problemSize];
			int i = 0;
			while ((pos = part1.find(delimiter2)) != std::string::npos) {
				token = part1.substr(0, pos);
				//std::cout << token << std::endl;
				//sscanf(token.c_str(), "%d", &temparray[i]);
				stringstream(token) >> temparray[i];
				i++;
				part1.erase(0, pos + delimiter.length());
			}
			stringstream(part1) >> temparray[i];
			/*cout << "temparray ..." << endl;
			for (int i = 0; i < pbsize; i++) {
				cout << temparray[i] << " ";
			}*/
			//cout << "remaining: " << part1 << endl;
			m_identityPermutations.push_back(temparray);
		}
		//cout << "line" << line << endl;
		countlines++;
	}

	// Initialize identity m_permutation
	m_identityPerm = new int[m_problemSize];
	for (int i = 0; i < m_problemSize; i++) {
		m_identityPerm[i] = i;
	}

}

void DynPermP::setPbsize(int size) {
	this->m_problemSize = size;
}

bool DynPermP::changeIdentityPermutation(int fes, int maxfes) {
	if (m_nextChangeIndex < m_numChanges) {
		int nextChangeFes = (int)rint(m_normalisedChangeGenerations[m_nextChangeIndex] * (double)maxfes);
		if (fes >= nextChangeFes) {
			m_identityPerm = m_identityPermutations[m_nextChangeIndex];
//			Tools::printPermutationWithPrefix(m_identityPerm, m_problemSize, "Changed identity perm: ");
			m_nextChangeIndex++;
			return true;
		}
	}
	return false;
}
