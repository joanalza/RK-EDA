/* 
 * File:   RK.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:27
 */
#include "Tools.h"
#include "EDAUtils.h"
#include <iostream>
#include <cstring>

#ifndef RK_H
#define	RK_H
class RK {
public:
	
	RK();
	RK(double randomkeys1[], int psize);
	virtual ~RK();
   
	double getFitness();
	void setFitness(double fitness1);
	void setProblemSize(int size);
	void setRandomKeys(double* rks);

	void setPermutation(int* perm);

	int getProblemSize();

	int* getPermutation();

	double* getRandomKeys();

	double* copyGene();

//	string getPermutationAsString();

//	string getRandomKeyAsString();

	RK copyOf();

	void normalise();
	double* getNormalisedRKs();
	void setRKfromPermutation(int* perm);
	RK* Clone();
	void deepCopy(RK* obj1);
	void setPermFromRKs() {
		double* ordered_rks = new double[m_pSize];
		bool found = false;
		int i, j = 0;
		for (i = 0; i < m_pSize; i++) {
			ordered_rks[i] = m_randomkeys[i];
		}
		//		Tools::printRK(ordered_rks, problemSize);
		sort(ordered_rks, ordered_rks + m_pSize);
		//		Tools::printRK(ordered_rks, problemSize);
		for (i = 0; i < m_pSize; i++) {
			found = false;
			while (!found) {
				if (ordered_rks[j] == m_randomkeys[i]) {
					m_permutation[i] = j;
					found = true;
					j = 0;
				}
				else
					j++;
			}
		}
		delete[] ordered_rks;
	}

	int m_pSize;
	double* m_randomkeys;
	int* m_permutation;
	double m_fitness;
	private:


};


#endif	/* RK_H */

